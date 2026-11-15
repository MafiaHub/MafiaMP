#pragma once

#include <cstdint>

#include "c_weak_ptr_proxy.h"
#include "s_single_threaded_policy.h"

namespace SDK {
    namespace ue {
        // ue::C_WeakPtr<T, TPolicy, TKind>. The general template is the TKind = 1 form the older callers
        // use (scene objects, databases). TODO: its layout predates the layout rules and is unverified.
        template <typename T, typename TPolicy = S_SingleThreadedPolicy, int TKind = 1>
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

        // TKind = 0: one pointer to the object's shared proxy, holding a reference to it. Passed by
        // value, it is a temporary the callee releases, so a copy hands the game a reference of its own.
        template <typename T>
        class C_WeakPtr<T, S_SingleThreadedPolicy, 0> {
          public:
            C_WeakPtr() = default;

            C_WeakPtr(C_WeakPtr const &other): m_pProxy(other.m_pProxy) {
                if (m_pProxy) {
                    m_pProxy->AddRef();
                }
            }

            C_WeakPtr &operator=(C_WeakPtr const &other) {
                if (other.m_pProxy) {
                    other.m_pProxy->AddRef();
                }
                Reset();
                m_pProxy = other.m_pProxy;
                return *this;
            }

            ~C_WeakPtr() {
                Reset();
            }

            T *Get() const {
                return m_pProxy ? m_pProxy->Get() : nullptr;
            }

            void Reset() {
                if (m_pProxy) {
                    m_pProxy->Release();
                    m_pProxy = nullptr;
                }
            }

          private:
            C_WeakPtrProxy<T> *m_pProxy = nullptr; // 0x00 - 0x08
        };
        static_assert(sizeof(C_WeakPtr<C_WeakPtrProxyBase, S_SingleThreadedPolicy, 0>) == 0x08, "C_WeakPtr<T, S_SingleThreadedPolicy, 0> size mismatch");
    } // namespace ue
} // namespace SDK
