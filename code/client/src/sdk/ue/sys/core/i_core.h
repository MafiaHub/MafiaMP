#pragma once

#include "c_core.h"

#include <stdint.h>

namespace SDK {
    namespace ue::sys::core {
        class I_Core {
          public:
            ~I_Core() = default;
            static C_Core *GetInstance();
            static bool IsInitialized() {
                return GetInstance() != nullptr;
            }
        };
    } // namespace ue::sys::core
} // namespace SDK