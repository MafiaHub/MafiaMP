#pragma once

#include <stdint.h>

namespace SDK {
    namespace ue::sys::render::device {
        enum class E_VSyncMode : uint32_t {
            Off      = 0,
            On       = 1,
            Adaptive = 2,
        };
    } // namespace ue::sys::render::device
} // namespace SDK
