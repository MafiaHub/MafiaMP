#pragma once

#include <cstddef>
#include <cstdint>

namespace SDK {
    namespace ue {
        // Intrusive count that ue::C_CntPtr holds. Not atomic: the game counts it with plain increments.
        class C_RefPtrCounter {
          public:
            virtual ~C_RefPtrCounter() = default;
            virtual void Destroy() const = 0;

            void AddRef() {
                ++m_nRefCount;
            }

            void Release() {
                if (--m_nRefCount == 0) {
                    Destroy();
                }
            }

          private:
            int32_t m_nRefCount; // 0x08 - 0x0C
            char _padC[0x4];     // 0x0C - 0x10

            static void AssertLayout() {
                static_assert(sizeof(C_RefPtrCounter) == 0x10, "C_RefPtrCounter size mismatch");
                static_assert(offsetof(C_RefPtrCounter, m_nRefCount) == 0x08, "C_RefPtrCounter::m_nRefCount offset mismatch");
            }
        };
    } // namespace ue
} // namespace SDK
