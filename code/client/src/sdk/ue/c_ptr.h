#pragma once

#include <cstdint>

namespace SDK {
    namespace ue {
        template <typename T>
        class C_Ptr {
          public:
            uintptr_t unk = 0;
            T *ptr        = nullptr;

            inline T &operator*() {
                return ptr;
            }
            inline T *operator->() {
                return ptr;
            }
        };
    } // namespace ue
} // namespace SDK
