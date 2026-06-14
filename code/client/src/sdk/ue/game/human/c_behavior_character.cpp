#include "c_behavior_character.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::human {
        void C_BehaviorCharacter::SetWAnimVariable(ue::game::anim::E_WAnimBehaviorVar id, float value) {
            hook::this_call<void>(gPatterns.C_BehaviorCharacter__SetWAnimVariable_float, this, static_cast<int>(id), value);
        }

        float C_BehaviorCharacter::GetWAnimVariable(ue::game::anim::E_WAnimBehaviorVar id) const {
            return hook::this_call<float>(gPatterns.C_BehaviorCharacter__GetWAnimVariable_float, this, static_cast<int>(id));
        }
    } // namespace ue::game::human
} // namespace SDK
