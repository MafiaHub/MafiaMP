#pragma once

#include <cstdint>

namespace SDK {
    namespace ue {
        template <typename T>
        class C_WeakPtr {
          public:
            T *Get() const {
                return m_Ptr;
            }

            bool IsValid() const {
                return m_Ptr != nullptr;
            }

          public:
            uintptr_t unk;
            T *m_Ptr = nullptr;
            uintptr_t unk3;
            uintptr_t unk4;
            uint8_t paaddd[32];
        };
    } // namespace ue
} // namespace SDK
