#pragma once

#include <cstddef>

#include "s_human_ai_request.h"

namespace SDK {
    namespace game::ai {
        // Plays an animation by name; the controller serves it with C_CharacterStateHandlerPlayAnimation.
        class S_HumanAIRequestPlayAnimNamed: public S_HumanAIRequest {
          public:
            // The request the play-animation AI action builds for a plain PlayAnimation script call,
            // allocated on the game's heap and holding one reference.
            static S_HumanAIRequestPlayAnimNamed *Create(char const *animationName, bool loop);

          private:
            char _pad30[0x1A8 - 0x30]; // 0x30 - 0x1A8

            static void AssertLayout() {
                static_assert(sizeof(S_HumanAIRequestPlayAnimNamed) == 0x1A8, "S_HumanAIRequestPlayAnimNamed size mismatch");
            }
        };
    } // namespace game::ai
} // namespace SDK
