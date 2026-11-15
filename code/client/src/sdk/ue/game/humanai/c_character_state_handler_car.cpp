#include "c_character_state_handler_car.h"

#include <sdk/patterns.h>

namespace SDK {
    namespace ue::game::humanai {
        void C_CharacterStateHandlerCar::GoToCarState(E_CAR_STATE state, bool forceChange) {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerCar__GoToCarState, this, state, forceChange);
        }
    } // namespace ue::game::humanai
} // namespace SDK
