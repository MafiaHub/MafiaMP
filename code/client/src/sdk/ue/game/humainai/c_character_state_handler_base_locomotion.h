#pragma once

#include "c_character_state_handler.h"

namespace SDK {
    namespace ue::game::humanai {
        class C_CharacterStateHandlerBaseLocomotion: public C_CharacterStateHandler {
          public:
            bool Idle2MoveTransitionActive(ue::game::anim::S_WAnimStateHandle const *) const;
            static void AddRemoveSprintDescriptor(C_CharacterController *, bool sprinting);
        };
    }
}
