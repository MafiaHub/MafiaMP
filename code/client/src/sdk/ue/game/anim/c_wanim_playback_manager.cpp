#include "c_wanim_playback_manager.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::anim {
        S_WAnimStateHandle C_WAnimPlaybackManager::PlayState(ue::sys::utils::C_HashName const &state, unsigned int a3, float a4, int a5, void *pOnDemandNodeParams) {
            S_WAnimStateHandle handle;
            (*(void(__thiscall *)(C_WAnimPlaybackManager *, S_WAnimStateHandle &, ue::sys::utils::C_HashName const &, unsigned int, float, int,
                                  void *))gPatterns.C_WAnimPlaybackManager__PlayState)(this, handle, state, a3, a4, a5, pOnDemandNodeParams);
            return handle;
        }
    } // namespace ue::game::anim
} // namespace SDK
