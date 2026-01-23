#include "c_human_head_controller.h"

#include "../../patterns.h"

namespace SDK {
    void C_HumanHeadController::ShutUp() {
        hook::this_call(gPatterns.C_HumanHeadController__ShutUp, this);
    }

    void C_HumanHeadController::Update() {
        hook::this_call(gPatterns.C_HumanHeadController__Update, this);
    }
} // namespace SDK
