#pragma once

#include "c_character_state_handler_base_locomotion.h"

namespace SDK {
    namespace ue::game::humanai {
        class C_CharacterStateHandlerMove: public C_CharacterStateHandlerBaseLocomotion {
          public:
            bool IsSprinting() const;
            bool SharpTurnTransitionActive();
        };
    }
}
