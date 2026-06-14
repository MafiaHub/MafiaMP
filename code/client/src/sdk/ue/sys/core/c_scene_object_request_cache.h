#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::sys::core {
        class C_SceneObject;

        class C_SceneObjectRequestCache {
          public:
            struct C_Cache {
                uint64_t m_nCacheId;       // 0x00 - 0x08
                void *m_pCacheData;        // 0x08 - 0x10
                void *m_pCacheEnd;         // 0x10 - 0x18
                uint64_t m_nRequestFlags;  // 0x18 - 0x20
                uint64_t m_nMaxRequests;   // 0x20 - 0x28
                bool m_bActive;            // 0x28 - 0x29
                char _pad29[0x47];         // 0x29 - 0x70
            };

          protected:
            uint64_t m_nNextCacheId;       // 0x00 - 0x08
            C_Cache *m_pCachesStart;       // 0x08 - 0x10
            C_Cache *m_pCachesEnd;         // 0x10 - 0x18
            C_Cache *m_pCachesCapacity;    // 0x18 - 0x20
            void *m_pLookupMapRoot;        // 0x20 - 0x28
            void *m_pLookupMapEnd;         // 0x28 - 0x30
            uint64_t m_nLookupMapSize;     // 0x30 - 0x38
            void *m_pEventListRoot;        // 0x38 - 0x40
            void *m_pEventListEnd;         // 0x40 - 0x48
            uint64_t m_nEventListSize;     // 0x48 - 0x50
        };
    }; // namespace ue::sys::core
} // namespace SDK
