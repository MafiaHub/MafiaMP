#pragma once

#include "c_character_state_handler.h"

namespace SDK {
    namespace ue::game::humanai {
        class C_CharacterStateHandlerPlayOverlayAnimation : public C_CharacterStateHandler {
          public:
            void StopAllOverlaysFromScript();
        };
    } // namespace ue::game::humanai
} // namespace SDK
