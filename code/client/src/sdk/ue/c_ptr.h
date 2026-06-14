#pragma once

#include <cstdint>

namespace SDK {
    namespace ue {
        template <typename T>
        class C_Ptr {
          public:
            T *ptr        = nullptr;  // 0x00 - 0x08
            uintptr_t unk = 0;        // 0x08 - 0x10

            inline T &operator*() {
                return *ptr;
            }
            inline T *operator->() {
                return ptr;
            }
            inline T *Get() const {
                return ptr;
            }
        };
    } // namespace ue
} // namespace SDK
