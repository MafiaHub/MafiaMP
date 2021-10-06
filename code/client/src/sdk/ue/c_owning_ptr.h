#pragma once

#include <cstdint>

#include "c_ptr.h"

namespace SDK {
    namespace ue {
        template <typename T>
        class C_OwningPtr: public C_Ptr<T> {
          public:
            uintptr_t unk2 = 0;
        };
    }
} // namespace SDK
