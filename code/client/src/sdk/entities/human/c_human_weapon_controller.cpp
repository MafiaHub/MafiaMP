#include "c_human_weapon_controller.h"

#include <logging/logger.h>
#include <utils/hooking/hooking.h>

#include "../../patterns.h"

namespace SDK {
    int C_HumanWeaponController::GetRightHandWeaponID() {
        return hook::this_call<int>(gPatterns.C_HumanWeaponController__GetRightHandWeaponID, this);
    }

    void C_HumanWeaponController::GetShotPosDir(SDK::ue::sys::math::C_Vector *OutPos, SDK::ue::sys::math::C_Vector *OutDir, float *Unknown) {
        hook::this_call<void>(gPatterns.C_HumanWeaponController__GetShotPosDir, this, OutPos, OutDir, Unknown);
    }

    bool C_HumanWeaponController::IsThrownWeapon() {
        return hook::this_call<int>(gPatterns.C_HumanWeaponController__IsThrownWeapon, this);
    }

} // namespace SDK
