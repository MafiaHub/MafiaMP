#pragma once

#include <cstdint>

namespace SDK {
    namespace ue {
        template <typename T>
        class C_WeakPtr {
          public:
            uintptr_t unk;
            uintptr_t unk2;
            uintptr_t unk3;
            uintptr_t unk4;
            uint8_t paaddd[32];
        };
    }
} // namespace SDK
