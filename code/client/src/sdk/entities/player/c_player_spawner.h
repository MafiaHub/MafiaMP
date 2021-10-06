#pragma once

#include <cstdint>

namespace SDK {
    class C_PlayerSpawner {
      public:
        uint64_t GetSpawnProfile() {
            return *(uint64_t *)(((uintptr_t)this) + 128);
        }
    };
}
