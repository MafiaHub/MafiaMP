#pragma once

#include <cstdint>

namespace SDK {
    namespace ue {
        template <typename T>
        class C_CntPtr {
          public:
            T *ptr = nullptr;
            uint32_t refCount;
            inline T &operator*() {
                return ptr;
            }
            inline T *operator->() {
                return ptr;
            }
        };
    } // namespace ue
} // namespace SDK
