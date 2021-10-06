#include "c_behavior_character.h"

#include <utils/hooking/hooking.h>
#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::human {
        void C_BehaviorCharacter::SetWAnimVariable(int id, float value) {
            hook::this_call<void>(gPatterns.C_BehaviorCharacter__SetWAnimVariable_float, this, id, value);
        }
    } // namespace ue::game::human
}
