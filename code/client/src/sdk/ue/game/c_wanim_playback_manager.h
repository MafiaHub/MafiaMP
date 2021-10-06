#pragma once
#include "s_wanim_state_handle.h"

#include "../sys/utils/c_hash_name.h"

namespace SDK {
    namespace ue::game::anim {
        class C_WAnimPlaybackManager {
          public:
            S_WAnimStateHandle PlayState(ue::sys::utils::C_HashName const &, unsigned int, float, int, void *pOnDemandNodeParams = nullptr);
        };
    } // namespace ue::game::anim
} // namespace SDK
