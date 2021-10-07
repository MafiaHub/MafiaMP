#pragma once

#include "i_render_device.h"

#include <d3d11.h>

namespace SDK {
    namespace ue::sys::render::device {
        class C_Direct3D11RenderDevice: public I_RenderDevice {
          public:
            char pad_0[0xC8];                       // 0000 - 00C8
            D3D_FEATURE_LEVEL _featureLevel;        // 00C8 - 00D0
            ID3D11Device *_device;                  // 00D0 - 00D8
            D3D11_DEVICE_CONTEXT_TYPE _contextType; // 00D8 
        };
    } // namespace ue::sys::render::device
} // namespace SDK
