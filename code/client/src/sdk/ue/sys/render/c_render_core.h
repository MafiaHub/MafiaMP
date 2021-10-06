#pragma once

#include "device/i_render_device.h"

namespace SDK {
    namespace ue::sys::render {
        class C_RenderCore {
          public:
            char pad0[0x38];
            ue::sys::render::device::I_RenderDevice *m_pRenderDevice; // 0038 - 0040
        };
    } // namespace ue::sys::render
} // namespace SDK