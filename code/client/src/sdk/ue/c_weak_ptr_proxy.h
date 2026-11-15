#pragma once

#include "c_weak_ptr_proxy_base.h"

namespace SDK {
    namespace ue {
        template <typename T>
        class C_WeakPtrProxy: public C_WeakPtrProxyBase {
          public:
            T *Get() const {
                return m_pObject;
            }

          private:
            T *m_pObject; // 0x10 - 0x18, null once the object is gone
        };
        static_assert(sizeof(C_WeakPtrProxy<C_WeakPtrProxyBase>) == 0x18, "C_WeakPtrProxy size mismatch");
    } // namespace ue
} // namespace SDK
