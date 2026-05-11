#pragma once

namespace SDK {
    namespace ue::sys::render {
        // Engine-side render-destination wrapper. Created by C_RenderCore::CreateDefaultRenderDestination
        // (M1DE 0x143A913A0) and stored in C_RenderCore::m_pDefaultRenderContext.
        // Full layout not reversed; we only need the type as a forward.
        class C_RenderContext;
    } // namespace ue::sys::render
} // namespace SDK
