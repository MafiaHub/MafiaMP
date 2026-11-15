#pragma once

#include <cstddef>

#include <intrin.h>

namespace SDK {
    namespace ue {
        // The block every weak pointer to one object shares. The object owns one reference and clears
        // the proxy's object pointer when it dies; each weak pointer owns another.
        class C_WeakPtrProxyBase {
          public:
            virtual ~C_WeakPtrProxyBase() = default;

            void AddRef() {
                _InterlockedIncrement(&m_nRefCount);
            }

            // The last reference frees the proxy through the game's deleting destructor.
            void Release() {
                if (_InterlockedDecrement(&m_nRefCount) == 0) {
                    delete this;
                }
            }

          private:
            volatile long m_nRefCount; // 0x08 - 0x0C
            char _padC[0x4];           // 0x0C - 0x10

            static void AssertLayout() {
                static_assert(sizeof(C_WeakPtrProxyBase) == 0x10, "C_WeakPtrProxyBase size mismatch");
                static_assert(offsetof(C_WeakPtrProxyBase, m_nRefCount) == 0x08, "C_WeakPtrProxyBase::m_nRefCount offset mismatch");
            }
        };
    } // namespace ue
} // namespace SDK
