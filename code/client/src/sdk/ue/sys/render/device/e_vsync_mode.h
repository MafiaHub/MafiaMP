#pragma once

#include <stdint.h>

namespace SDK {
    namespace ue::sys::render::device {
        // E_VSyncMode is the 5th argument type of I_RenderDevice::Present.
        // The TYPE NAME comes from a cross-platform M3 demangled symbol; the underlying
        // enum VALUES below are DXGI-conventional guesses and have NOT been verified
        // against the M1DE binary. Treat them as opaque when round-tripping through the
        // engine until we observe the actual integer values in a Present call.
        enum class E_VSyncMode : uint32_t {
            Off      = 0,
            On       = 1,
            Adaptive = 2,
        };
    } // namespace ue::sys::render::device
} // namespace SDK
