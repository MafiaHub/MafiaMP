#pragma once

#include "../ue/c_ptr.h"

#include <cstdint>
#include <vector>

namespace SDK {
    namespace prefab {
        // Forward declarations
        class I_PrefabWorker;
        struct S_GlobalInitData;
        class C_PrefabManager;

        enum E_PREFAB_TYPE : int32_t {
            E_PT_Base             = 0,
            // 1 = unused/reserved
            E_PT_Car              = 2,
            E_PT_CrashObj         = 3,
            E_PT_ActorDeform      = 4,
            E_PT_Wheel            = 5,
            // 6 = unused/reserved
            E_PT_Door             = 7,
            E_PT_Obsolete_Lift    = 8,
            E_PT_Boat             = 9,
            E_PT_Obsolete_Wagon   = 10,
            E_PT_AIBrain          = 11,
            E_PT_MountedWeapon    = 12,
            E_PT_TrafficSemaphore = 13,
            E_PT_None             = 14,
        };

        // Base class for prefab init data with reference counting
        struct S_GlobalInitData {
            virtual ~S_GlobalInitData() = default;
        };

        // Interface for prefab worker - handles prefab creation
        class I_PrefabWorker {
          public:
            virtual ~I_PrefabWorker()                                                  = default;
            virtual ue::C_Ptr<S_GlobalInitData> CreatePrefab(E_PREFAB_TYPE type) const = 0;
        };

        // Key for binary search in prefab array - 16 bytes
        struct S_PrefabKey {
            uint64_t m_uGUID;       // 0x00 - 0x08
            E_PREFAB_TYPE m_eType;  // 0x08 - 0x0C
            char _pad0[0x4];        // 0x0C - 0x10
        };

        static_assert(sizeof(S_PrefabKey) == 16, "Wrong size: prefab::S_PrefabKey");

        // Entry in the sorted prefab array - 48 bytes
        // Sorted by (guid, type) for binary search
        struct S_PrefabEntry {
            uint64_t m_uGUID;                            // 0x00 - 0x08
            E_PREFAB_TYPE m_eType;                       // 0x08 - 0x0C
            char _pad0[0x4];                             // 0x0C - 0x10
            std::vector<S_GlobalInitData *> m_aData;     // 0x10 - 0x28
            bool m_bOwned;                               // 0x28 - 0x29
            char _pad1[0x7];                             // 0x29 - 0x30

            // Insert entry into sorted array
            static S_PrefabEntry *Insert(C_PrefabManager *manager, const S_PrefabEntry &entry);

            // Move range of entries
            static void MoveRange(S_PrefabEntry *dst, S_PrefabEntry *srcEnd, S_PrefabEntry *dstEnd);
        };

        static_assert(sizeof(S_PrefabEntry) == 48, "Wrong size: prefab::S_PrefabEntry");

        // Streaming context for prefab loading - 24 bytes
        struct S_StreamingContext {
            float m_fLoadFactor;       // 0x00 - 0x04 (default 1.0f)
            char _pad0[0x4];           // 0x04 - 0x08
            void *m_pStreamingNode;    // 0x08 - 0x10
            void *m_pUnk10;            // 0x10 - 0x18
        };

        static_assert(sizeof(S_StreamingContext) == 24, "Wrong size: prefab::S_StreamingContext");

        // Resource notification listener interface
        class I_ResourceListener {
          public:
            virtual ~I_ResourceListener() = default;
            virtual void OnResourceActivated()   {}
            virtual void OnResourceDeactivated() {}
        };

        // Singleton prefab manager - 184 bytes (0xB8)
        //
        // Reversed from:
        // - Constructor: 0x14173e2f0
        // - Destructor: 0x141747790
        // - GetInstance: 0x14178b110
        // - GetPrefab: 0x141793a20
        // - InsertPrefab: 0x1417b2690
        // - RemovePrefab: 0x1417a34f0
        // - MakePrefabOnHeap: 0x1417a97b0
        // - BinarySearchSlot: 0x1417c3c70
        class C_PrefabManager {
          public:
            static C_PrefabManager *GetInstance();

            S_GlobalInitData *GetPrefab(const uint64_t &guid, E_PREFAB_TYPE type);
            S_GlobalInitData *GetPrefab(const uint64_t &guid, unsigned int type);
            void InsertPrefab(const uint64_t &guid, E_PREFAB_TYPE type, ue::C_Ptr<S_GlobalInitData> initData);
            void RemovePrefab(const uint64_t &guid, E_PREFAB_TYPE type, S_GlobalInitData *initData);
            void RemovePrefabs(E_PREFAB_TYPE type);
            ue::C_Ptr<S_GlobalInitData> MakePrefabOnHeap(const uint64_t &guid, E_PREFAB_TYPE type);
            void ReleaseAll();
            void SetPrefabWorker(I_PrefabWorker *worker);
            void Shutdown();

          private:
            C_PrefabManager();
            ~C_PrefabManager();

            C_PrefabManager(const C_PrefabManager &)            = delete;
            C_PrefabManager &operator=(const C_PrefabManager &) = delete;

            void RemoveHeapPrefabData(S_GlobalInitData *data);
            S_PrefabEntry *BinarySearchSlot(const S_PrefabKey &key);

            static C_PrefabManager *m_PrefabManagerSingleton;

            I_PrefabWorker *m_pPrefabWorker;                   // 0x00 - 0x08
            std::vector<S_PrefabEntry> m_aPrefabs;             // 0x08 - 0x20
            char _pad0[0x8];                                   // 0x20 - 0x28
            std::vector<S_GlobalInitData *> m_aHeapPrefabs;    // 0x28 - 0x40
            I_ResourceListener *m_pResourceListener;           // 0x40 - 0x48
            S_StreamingContext m_sStreamingContext;            // 0x48 - 0x60
            std::vector<S_GlobalInitData *> m_aOwnedPrefabs;   // 0x60 - 0x78
            char _pad1[0x10];                                  // 0x78 - 0x88
            char m_sMutex[0x30];                               // 0x88 - 0xB8
        };

        static_assert(sizeof(C_PrefabManager) == 184, "Wrong size: prefab::C_PrefabManager");
    } // namespace prefab
} // namespace SDK
