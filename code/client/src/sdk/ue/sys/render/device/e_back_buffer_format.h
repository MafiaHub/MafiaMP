#pragma once

#include <stdint.h>

namespace SDK {
    namespace ue::sys::render::device {
        // Engine-internal back-buffer format enum. Values match the constants used by
        // C_Direct3D11RenderDevice / C_RenderDeviceNULL GetBackBufferFormat() and the
        // S_VideoModeDesc.m_nFormat field (M3 == 20, M1DE == 21).
        enum class E_BackBufferFormat : uint32_t {
            Unknown   = 0,
            R8G8B8A8  = 20,
            R10G10B10 = 21,
        };
    } // namespace ue::sys::render::device
} // namespace SDK
