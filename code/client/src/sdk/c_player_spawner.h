#pragma once

#include <cstdint>

namespace SDK {
    class C_PlayerSpawner {
      public:
        char pad0[0x78];            // 0008 - 0080
        uint64_t m_iSpawnProfile;   // 0080 - 0088

      public:
        virtual ~C_PlayerSpawner() = default;

        uint64_t GetSpawnProfile() {
            return m_iSpawnProfile;
        }
    };
}
