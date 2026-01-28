#pragma once

#include "../anim/c_wanim_playback_manager.h"
#include "../anim/e_wanim_behavior_var.h"

namespace SDK {
    namespace ue::game::human {
        class C_BehaviorCharacter {
          private:
            uint8_t pad[120];
            ue::game::anim::C_WAnimPlaybackManager *m_pWAnimPlaybackManager;

          public:
            ue::game::anim::C_WAnimPlaybackManager *GetWAnimPlaybackManager() const {
                return m_pWAnimPlaybackManager;
            }

            void SetWAnimVariable(ue::game::anim::E_WAnimBehaviorVar id, float value);
            float GetWAnimVariable(ue::game::anim::E_WAnimBehaviorVar id) const;
        };
    } // namespace ue::game::human
}
