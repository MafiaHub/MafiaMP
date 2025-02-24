#include "c_human_weapon_controller.h"

#include <logging/logger.h>

#include "../../patterns.h"

namespace SDK {
    /**
     * @param unk If false, the model does not appear, maybe for scripted weapons (like turret_truck)
     */
    bool C_HumanWeaponController::DoWeaponSelectByItemId(unsigned int weaponId, bool unk) {
        return hook::this_call<bool>(gPatterns.C_HumanWeaponController__DoWeaponSelectByItemId, this, weaponId, unk);
    }

    void C_HumanWeaponController::GetShotPosDir(SDK::ue::sys::math::C_Vector *outPosition, SDK::ue::sys::math::C_Vector *outDirection, float *unknown) {
        hook::this_call<void>(gPatterns.C_HumanWeaponController__GetShotPosDir, this, outPosition, outDirection, unknown);
    }

    bool C_HumanWeaponController::IsThrownWeapon() {
        return hook::this_call<bool>(gPatterns.C_HumanWeaponController__IsThrownWeapon, this);
    }

    void C_HumanWeaponController::ResetScatterCoef() {
        hook::this_call<void>(gPatterns.C_HumanWeaponController__ResetScatterCoef, this);
    }

    void C_HumanWeaponController::SetAiming(const bool bIsActive) {
        hook::this_call<void>(gPatterns.C_HumanWeaponController__SetAiming, this, bIsActive);
    }

    bool C_HumanWeaponController::IsAiming() {
        return (*(BYTE *)(this + 892) & 1) != 0;
    }

    void C_HumanWeaponController::SetCoverFlag(const bool bIsActive) {
        hook::this_call<void>(gPatterns.C_HumanWeaponController__SetCoverFlag, this, bIsActive);
    }

    void C_HumanWeaponController::SetFirePressedFlag(const bool bIsActive) {
        hook::this_call<void>(gPatterns.C_HumanWeaponController__SetFirePressedFlag, this, bIsActive);
    }

    void C_HumanWeaponController::SetStickMove(const ue::sys::math::C_Vector2 &movementDelta) {
        hook::this_call<void>(gPatterns.C_HumanWeaponController__SetStickMove, this, movementDelta);
    }

    void C_HumanWeaponController::SetZoomFlag(const bool bIsActive) {
        hook::this_call<void>(gPatterns.C_HumanWeaponController__SetZoomFlag, this, bIsActive);
    }

    bool C_HumanWeaponController::DoShot(void *unk, ue::sys::math::C_Vector *vec1, ue::sys::math::C_Vector *vec2, bool unk1, bool unk2) {
        return hook::this_call<bool>(gPatterns.C_HumanWeaponController__DoShot, this, unk, vec1, vec2, unk1, unk2);
    }

    bool C_HumanWeaponController::DoWeaponReloadInventory(int unk) {
        return hook::this_call<bool>(gPatterns.C_HumanWeaponController__DoWeaponReloadInventory, this, unk);
    }
} // namespace SDK
