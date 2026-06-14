#pragma once

#include "i_render_device.h"

namespace SDK {
    namespace ue::sys::render::device {
        // Fallback "no-op" implementation of I_RenderDevice. Used by the engine before
        // a real device is created and as a safe default when D3D11 init fails.
        // GetBackBufferWidth/Height return 1280/720 constants in M3 Mac; the M1DE
        // build inherits the same constants.
        class C_RenderDeviceNULL: public I_RenderDevice {};
    } // namespace ue::sys::render::device
} // namespace SDK
