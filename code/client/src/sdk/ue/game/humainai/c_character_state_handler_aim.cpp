#include "c_character_state_handler_aim.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::humanai {
        bool C_CharacterStateHandlerAim::SwappingWeapon() {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerAim__SwappingWeapon, this);
        }

        bool C_CharacterStateHandlerAim::UpdateAimAnimation() {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerAim__UpdateAimAnimation, this);
        }
    }
}
