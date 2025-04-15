#include "c_character_state_handler_base_locomotion.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::humanai {
        bool C_CharacterStateHandlerBaseLocomotion::Idle2MoveTransitionActive(ue::game::anim::S_WAnimStateHandle const *pWAnimStateHandle) const {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerBaseLocomotion__Idle2MoveTransitionActive, this, pWAnimStateHandle);
        }

        void C_CharacterStateHandlerBaseLocomotion::AddRemoveSprintDescriptor(C_CharacterController *pCharCtrl, bool sprinting) {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerBaseLocomotion__AddRemoveSprintDescriptor, pCharCtrl, sprinting);
        }
    }
}
