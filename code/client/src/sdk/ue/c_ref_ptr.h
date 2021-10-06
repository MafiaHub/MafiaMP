#pragma once

#include <cstdint>

namespace SDK {
    namespace ue {
        template <typename T>
        class C_RefPtr {
          public:
            uint32_t _0;
            uint16_t _4;
            uint16_t _6;
            T *ptr;
            uintptr_t _16;
            uintptr_t _24;
            inline T &operator*() {
                return ptr;
            }
            inline T *operator->() {
                return ptr;
            }
        };
    }
}
