#pragma once

#include <cstdint>

namespace SDK {
    namespace ue {
        template <typename T>
        class C_WeakPtr {
          public:
            uintptr_t unk;
            T *m_Ptr = nullptr;
            uintptr_t unk3;
            uintptr_t unk4;
            uint8_t paaddd[32];

            T *Get() const {
                return m_Ptr;
            }

            bool IsValid() const {
                return m_Ptr != nullptr;
            }

            T *operator->() const {
                return m_Ptr;
            }
        };
    } // namespace ue
} // namespace SDK
