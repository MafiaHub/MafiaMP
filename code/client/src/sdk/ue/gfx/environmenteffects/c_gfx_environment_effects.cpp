#include "c_gfx_environment_effects.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::gfx::environmenteffects {
        int C_GfxEnvironmentEffects::GetDecal() const {
            return m_iDecal;
        }

        C_GfxEnvironmentEffects *C_GfxEnvironmentEffects::GetInstance() {
            return *reinterpret_cast<C_GfxEnvironmentEffects **>(gPatterns.C_GfxEnvironmentEffects_Instance);
        }
    } // namespace ue::gfx::environmenteffects
}
