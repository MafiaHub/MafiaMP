#pragma once

#include "../../../patterns.h"
#include "c_virtual_file_system.h"

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace SDK::ue::sys::filesystem {
    // VfsCache hash table entry - 16 bytes
    struct S_VfsCacheEntry {
        uint64_t m_uHash;        // 0x00 - 0x08
        uint32_t m_uStringOffset; // 0x08 - 0x0C
        uint32_t _pad0;          // 0x0C - 0x10
    };

    static_assert(sizeof(S_VfsCacheEntry) == 16, "Wrong size: S_VfsCacheEntry");

    // VfsCache structure at offset 2600 in C_VirtualFileSystemCache - 160 bytes
    struct S_VfsCache {
        void *m_pUnk0;                    // 0x00 - 0x08
        const char *m_pStringBuffer;      // 0x08 - 0x10
        void *m_pUnk10;                   // 0x10 - 0x18
        void *m_pUnk18;                   // 0x18 - 0x20
        uint64_t m_uPlaceholderHash;      // 0x20 - 0x28
        void *m_pUnk28;                   // 0x28 - 0x30
        void *m_pUnk30;                   // 0x30 - 0x38
        void *m_pUnk38;                   // 0x38 - 0x40
        void *m_pUnk40;                   // 0x40 - 0x48
        void *m_pUnk48;                   // 0x48 - 0x50
        void *m_pUnk50;                   // 0x50 - 0x58
        void *m_pUnk58;                   // 0x58 - 0x60
        void *m_pUnk60;                   // 0x60 - 0x68
        S_VfsCacheEntry *m_pEntriesStart; // 0x68 - 0x70
        S_VfsCacheEntry *m_pEntriesEnd;   // 0x70 - 0x78
        void *m_pUnk78;                   // 0x78 - 0x80
        void *m_pUnk80;                   // 0x80 - 0x88
        void *m_pUnk88;                   // 0x88 - 0x90
        void *m_pUnk90;                   // 0x90 - 0x98
        void *m_pUnk98;                   // 0x98 - 0xA0

        size_t GetEntryCount() const {
            if (!m_pEntriesStart || !m_pEntriesEnd) {
                return 0;
            }
            return m_pEntriesEnd - m_pEntriesStart;
        }

        const char *GetStringByOffset(uint32_t offset) const {
            if (!m_pStringBuffer) {
                return nullptr;
            }
            return m_pStringBuffer + offset;
        }

        const char *GetFileName(size_t index) const {
            if (index >= GetEntryCount()) {
                return nullptr;
            }
            return GetStringByOffset(m_pEntriesStart[index].m_uStringOffset);
        }

        uint64_t GetFileHash(size_t index) const {
            if (index >= GetEntryCount()) {
                return 0;
            }
            return m_pEntriesStart[index].m_uHash;
        }
    };

    static_assert(sizeof(S_VfsCache) == 160, "Wrong size: S_VfsCache");

    class C_VirtualFileSystemCache: public C_VirtualFileSystem {
      public:
        static C_VirtualFileSystemCache *GetInstance() {
            return hook::this_call<C_VirtualFileSystemCache *>(gPatterns.I_VirtualFileSystemCache__GetInstance);
        }

        S_VfsCache *GetVfsCache() {
            return reinterpret_cast<S_VfsCache *>(reinterpret_cast<char *>(this) + 2600);
        }

        const S_VfsCache *GetVfsCache() const {
            return reinterpret_cast<const S_VfsCache *>(reinterpret_cast<const char *>(this) + 2600);
        }

        size_t GetCachedFileCount() const {
            const auto *cache = GetVfsCache();
            return cache ? cache->GetEntryCount() : 0;
        }

        const char *GetCachedFileName(size_t index) const {
            const auto *cache = GetVfsCache();
            return cache ? cache->GetFileName(index) : nullptr;
        }

        uint64_t GetCachedFileHash(size_t index) const {
            const auto *cache = GetVfsCache();
            return cache ? cache->GetFileHash(index) : 0;
        }

        void EnumerateCachedFiles(const std::function<bool(size_t index, uint64_t hash, const char *fileName)> &callback) const {
            const auto *cache = GetVfsCache();
            if (!cache) {
                return;
            }

            const size_t count = cache->GetEntryCount();
            for (size_t i = 0; i < count; ++i) {
                const char *fileName = cache->GetFileName(i);
                uint64_t hash        = cache->GetFileHash(i);
                if (!callback(i, hash, fileName)) {
                    break;
                }
            }
        }

        std::vector<std::string> GetCachedFilesByPrefix(const std::string &prefix) const {
            std::vector<std::string> result;
            EnumerateCachedFiles([&](size_t, uint64_t, const char *fileName) {
                if (fileName && std::string(fileName).find(prefix) == 0) {
                    result.push_back(fileName);
                }
                return true;
            });
            return result;
        }

        std::vector<std::string> GetCachedFilesByExtension(const std::string &extension) const {
            std::vector<std::string> result;
            EnumerateCachedFiles([&](size_t, uint64_t, const char *fileName) {
                if (fileName) {
                    std::string name(fileName);
                    if (name.length() >= extension.length() && name.substr(name.length() - extension.length()) == extension) {
                        result.push_back(fileName);
                    }
                }
                return true;
            });
            return result;
        }
    };
} // namespace SDK::ue::sys::filesystem
