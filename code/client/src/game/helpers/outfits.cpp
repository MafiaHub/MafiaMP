#include <utils/safe_win32.h>

#include "outfits.h"

#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include <algorithm>
#include <cstring>
#include <unordered_map>

// The spawn profile database (C_SpawnProfileDB) keeps, per profile hash, a channel: weighted
// choices of sub-profiles, and the combinable packages to load. A sub-profile lists piece sets; the
// game wears one piece of each set that passes its chance. An outfit is a channel with a single
// choice whose sub-profile has one single-piece set per piece, so the game wears exactly those.
//
// Everything is allocated with the game's allocator and added under the database's lock: the game
// owns these objects from then on, as it owns the ones it loaded.
namespace MafiaMP::Game::Helpers::Outfits {
    namespace {
        // C_SpawnProfileDB
        constexpr size_t kDbPieceSets     = 0x60;  // std::vector<C_PieceSet *>
        constexpr size_t kDbProfilesById  = 0xA0;  // std::map<uint64_t, Channel *>
        constexpr size_t kDbLock          = 0x448; // CRITICAL_SECTION
        constexpr size_t kDbSharedChoices = 0x4E8; // std::vector<SubProfile *>

        // Channel (0x50)
        constexpr size_t kChannelSize     = 0x50;
        constexpr size_t kChannelId       = 0x00;
        constexpr size_t kChannelChoices  = 0x08; // std::vector<{uint32_t index; float weight}>
        constexpr size_t kChannelPackages = 0x20; // std::vector<uint64_t>
        constexpr size_t kChannelMinimum  = 0x3C;
        constexpr size_t kChannelDesired  = 0x40;
        constexpr size_t kChannelLoaded   = 0x48; // two counters kept by the loader

        // Sub-profile (C_SpawnProfile, 0x60)
        constexpr size_t kSubSize         = 0x60;
        constexpr size_t kSubPieceSets    = 0x00; // std::vector<uint32_t>
        constexpr size_t kSubRangeIndexes = 0x18;
        constexpr size_t kSubChannels     = 0x30;

        // C_PieceSet (0x50)
        constexpr size_t kSetSize         = 0x50;
        constexpr size_t kSetCounter      = 0x00;
        constexpr size_t kSetWeight       = 0x04;
        constexpr size_t kSetChance       = 0x08;
        constexpr size_t kSetPieces       = 0x10; // std::vector<uint64_t>, as declared
        constexpr size_t kSetLoadedPieces = 0x28; // std::vector<uint64_t>, those loaded

        using GetManager_t  = void *(*)();
        using GetDatabase_t = void **(*)(void *, void **);
        using New_t         = void *(*)(size_t);
        using InsertNode_t  = void *(*)(void *tree, void **result, bool addLeft, void *parent, const void *key, void *node);

        struct Natives {
            GetManager_t getManager   = nullptr;
            GetDatabase_t getDatabase = nullptr;
            New_t allocate            = nullptr;
            InsertNode_t insertNode   = nullptr;
        };

        // The target of the call instruction at `site`.
        template <typename T>
        T CallTarget(uint8_t *site) {
            return reinterpret_cast<T>(site + 5 + *reinterpret_cast<int32_t *>(site + 1));
        }

        // Resolved through call sites in unique functions: the functions themselves start like
        // many others.
        const Natives &GetNatives() {
            static const Natives natives = [] {
                Natives n;
                // C_ProfileSpawner::IsSpawnProfileLoaded fetches the database first.
                auto *loaded  = hook::get_pattern<uint8_t>("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8B C8 48 8D 54 24 ? E8 ? ? ? ? 4C 8D 83 ? ? ? ?");
                n.getManager  = CallTarget<GetManager_t>(loaded + 13);
                n.getDatabase = CallTarget<GetDatabase_t>(loaded + 26);
                // The database's package bookkeeping allocates a map node and inserts it.
                n.allocate   = CallTarget<New_t>(hook::get_pattern<uint8_t>("48 8B 1E 48 8B 38 E8 ? ? ? ?", 6));
                n.insertNode = CallTarget<InsertNode_t>(hook::get_pattern<uint8_t>("20 49 8D 8F B8 02 00 00 E8 ? ? ? ?", 8));
                return n;
            }();
            return natives;
        }

        uint8_t *Database() {
            const auto &n = GetNatives();
            void *out     = nullptr;
            return static_cast<uint8_t *>(*n.getDatabase(n.getManager(), &out));
        }

        template <typename T>
        T &At(uint8_t *base, size_t offset) {
            return *reinterpret_cast<T *>(base + offset);
        }

        // A buffer the game's std::vector can free: blocks of 4 KiB and more are 32-byte aligned
        // with the allocation stored just before them, as MSVC's allocator lays them out.
        void *AllocateBuffer(size_t bytes) {
            if (bytes < 0x1000) {
                return GetNatives().allocate(bytes);
            }
            constexpr size_t kNonUserSize          = 2 * sizeof(void *) + 31;
            auto *block                            = static_cast<uint8_t *>(GetNatives().allocate(bytes + kNonUserSize));
            auto *aligned                          = reinterpret_cast<uint8_t *>((reinterpret_cast<uintptr_t>(block) + kNonUserSize) & ~uintptr_t(31));
            reinterpret_cast<void **>(aligned)[-1] = block;
            return aligned;
        }

        // Sets the std::vector at `vec` to a fresh buffer holding `bytes` of `data`.
        void AssignVector(uint8_t *vec, const void *data, size_t bytes) {
            auto *buffer = static_cast<uint8_t *>(bytes ? AllocateBuffer(bytes) : nullptr);
            if (bytes) {
                std::memcpy(buffer, data, bytes);
            }
            At<uint8_t *>(vec, 0x00) = buffer;
            At<uint8_t *>(vec, 0x08) = buffer + bytes;
            At<uint8_t *>(vec, 0x10) = buffer + bytes;
        }

        // Appends a pointer to the game's std::vector<T *> at `vec` and returns its index. The old
        // buffer is left allocated: a thread that read it before the lock may still be on it.
        uint32_t PushPointer(uint8_t *vec, void *value) {
            auto &begin       = At<void **>(vec, 0x00);
            auto &end         = At<void **>(vec, 0x08);
            auto &capacity    = At<void **>(vec, 0x10);
            const size_t size = end - begin;
            if (end == capacity) {
                const size_t newCapacity = size + size / 2 + 64;
                auto **buffer            = static_cast<void **>(AllocateBuffer(newCapacity * sizeof(void *)));
                std::memcpy(buffer, begin, size * sizeof(void *));
                begin    = buffer;
                end      = buffer + size;
                capacity = buffer + newCapacity;
            }
            *end++ = value;
            return static_cast<uint32_t>(size);
        }

        // std::map node: left, parent, right, color, isnil, then the key and the mapped value.
        uint8_t *FindProfile(uint8_t *db, uint64_t hash) {
            auto *head = At<uint8_t *>(db, kDbProfilesById);
            auto *node = At<uint8_t *>(head, 0x08);
            while (!node[0x19]) {
                const uint64_t key = At<uint64_t>(node, 0x20);
                if (key == hash) {
                    return At<uint8_t *>(node, 0x28);
                }
                node = At<uint8_t *>(node, key < hash ? 0x10 : 0x00);
            }
            return nullptr;
        }

        void InsertProfile(uint8_t *db, uint64_t hash, uint8_t *channel) {
            auto *tree   = db + kDbProfilesById;
            auto *head   = At<uint8_t *>(tree, 0x00);
            auto *parent = head;
            auto *node   = At<uint8_t *>(head, 0x08);
            bool addLeft = true;
            while (!node[0x19]) {
                parent  = node;
                addLeft = hash < At<uint64_t>(node, 0x20);
                node    = At<uint8_t *>(node, addLeft ? 0x00 : 0x10);
            }
            auto *newNode                = static_cast<uint8_t *>(GetNatives().allocate(0x30));
            At<uint8_t *>(newNode, 0x00) = head;
            At<uint8_t *>(newNode, 0x08) = head;
            At<uint8_t *>(newNode, 0x10) = head;
            newNode[0x18]                = 0; // red
            newNode[0x19]                = 0;
            At<uint64_t>(newNode, 0x20)  = hash;
            At<uint8_t *>(newNode, 0x28) = channel;
            void *result                 = nullptr;
            GetNatives().insertNode(tree, &result, addLeft, parent, &At<uint64_t>(newNode, 0x20), newNode);
        }

        class DatabaseLock final {
          public:
            explicit DatabaseLock(uint8_t *db): _section(reinterpret_cast<CRITICAL_SECTION *>(db + kDbLock)) {
                EnterCriticalSection(_section);
            }
            ~DatabaseLock() {
                LeaveCriticalSection(_section);
            }

          private:
            CRITICAL_SECTION *_section;
        };

        // Outfits the server defined, by profile hash.
        struct Definition {
            uint64_t templateProfile = 0;
            std::vector<Piece> pieces;
        };
        std::unordered_map<uint64_t, Definition> gDefinitions;
    } // namespace

    void Define(uint64_t profile, uint64_t templateProfile, std::vector<Piece> pieces) {
        gDefinitions[profile] = {templateProfile, std::move(pieces)};
    }

    bool Ensure(uint64_t profile) {
        if (profile == 0) {
            return true;
        }
        auto *db = Database();
        if (!db) {
            return false;
        }
        {
            DatabaseLock lock(db);
            if (FindProfile(db, profile)) {
                return true;
            }
        }
        const auto it = gDefinitions.find(profile);
        return it != gDefinitions.end() && Register(it->second.pieces, it->second.templateProfile) == profile;
    }

    uint64_t Register(const std::vector<Piece> &pieces, uint64_t templateProfile) {
        auto *db = Database();
        if (!db || pieces.empty()) {
            return 0;
        }
        const uint64_t hash = Shared::Outfits::HashOf(pieces);
        DatabaseLock lock(db);
        if (FindProfile(db, hash)) {
            return hash;
        }

        // The template gives the body ranges, the flags the spawners filter on, and a piece set to
        // copy the fields we do not know of.
        auto *templateChannel = FindProfile(db, templateProfile);
        if (!templateChannel || At<uint8_t *>(templateChannel, kChannelChoices) == At<uint8_t *>(templateChannel, kChannelChoices + 8)) {
            return 0;
        }
        const uint32_t templateChoice = At<uint32_t *>(templateChannel, kChannelChoices)[0];
        auto *templateSub             = At<uint8_t **>(db, kDbSharedChoices)[templateChoice];
        if (At<uint8_t *>(templateSub, kSubPieceSets) == At<uint8_t *>(templateSub, kSubPieceSets + 8)) {
            return 0;
        }
        auto *templateSet = At<uint8_t **>(db, kDbPieceSets)[At<uint32_t *>(templateSub, kSubPieceSets)[0]];

        std::vector<uint32_t> setIndexes;
        std::vector<uint64_t> packages;
        for (const auto &piece : pieces) {
            auto *set = static_cast<uint8_t *>(GetNatives().allocate(kSetSize));
            std::memcpy(set, templateSet, kSetSize);
            At<uint32_t>(set, kSetCounter) = 0;
            At<uint32_t>(set, kSetWeight)  = 1;
            At<float>(set, kSetChance)     = 1.0f;
            AssignVector(set + kSetPieces, &piece.pieceId, sizeof(uint64_t));
            AssignVector(set + kSetLoadedPieces, &piece.pieceId, sizeof(uint64_t));
            setIndexes.push_back(PushPointer(db + kDbPieceSets, set));
            if (std::find(packages.begin(), packages.end(), piece.packageId) == packages.end()) {
                packages.push_back(piece.packageId);
            }
        }

        auto *sub = static_cast<uint8_t *>(GetNatives().allocate(kSubSize));
        std::memcpy(sub, templateSub, kSubSize);
        AssignVector(sub + kSubPieceSets, setIndexes.data(), setIndexes.size() * sizeof(uint32_t));
        const auto *ranges = At<uint8_t *>(templateSub, kSubRangeIndexes);
        AssignVector(sub + kSubRangeIndexes, ranges, At<uint8_t *>(templateSub, kSubRangeIndexes + 8) - ranges);
        AssignVector(sub + kSubChannels, nullptr, 0);
        const uint32_t subIndex = PushPointer(db + kDbSharedChoices, sub);

        auto *channel = static_cast<uint8_t *>(GetNatives().allocate(kChannelSize));
        std::memcpy(channel, templateChannel, kChannelSize);
        At<uint64_t>(channel, kChannelId) = hash;
        const struct {
            uint32_t index;
            float weight;
        } choice {subIndex, 1.0f};
        AssignVector(channel + kChannelChoices, &choice, sizeof(choice));
        AssignVector(channel + kChannelPackages, packages.data(), packages.size() * sizeof(uint64_t));
        At<uint32_t>(channel, kChannelMinimum)    = static_cast<uint32_t>(packages.size());
        At<uint32_t>(channel, kChannelDesired)    = static_cast<uint32_t>(packages.size());
        At<uint32_t>(channel, kChannelLoaded)     = 0;
        At<uint32_t>(channel, kChannelLoaded + 4) = 0;
        InsertProfile(db, hash, channel);

        Framework::Logging::GetLogger("Outfits")->info("Registered outfit {:016x}: {} pieces, {} packages", hash, pieces.size(), packages.size());
        return hash;
    }
} // namespace MafiaMP::Game::Helpers::Outfits
