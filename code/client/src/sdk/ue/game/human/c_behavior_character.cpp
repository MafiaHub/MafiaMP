#include "c_behavior_character.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::human {
        void C_BehaviorCharacter::SetWAnimVariable(int id, float value) {
            hook::this_call<void>(gPatterns.C_BehaviorCharacter__SetWAnimVariable_float, this, id, value);
        }

        float C_BehaviorCharacter::GetWAnimVariable(int id) const {
            return hook::this_call<float>(gPatterns.C_BehaviorCharacter__GetWAnimVariable_float, this, id);
        }
    } // namespace ue::game::human
} // namespace SDK
