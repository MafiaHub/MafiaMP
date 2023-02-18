#pragma once

#include <cstdint>

#include "../../ue/sys/math/c_vector.h"

namespace SDK {
    class C_HumanWeaponController {
      public:
        bool DoWeaponSelectByItemId(unsigned int, bool);

        int GetRightHandWeaponID();

        void GetShotPosDir(SDK::ue::sys::math::C_Vector *, SDK::ue::sys::math::C_Vector *, float *);

        bool IsThrownWeapon();

        void ResetScatterCoef();

        void SetAiming(const bool);

        void SetCoverFlag(const bool);

        void SetFirePressedFlag(const bool);

        bool DoShot(void *, ue::sys::math::C_Vector *, ue::sys::math::C_Vector *, bool, bool);

        // TODO: Crashes, could it be that I don't have a gamepad? Maybe pattern is wrong?
        void SetStickMove(const ue::sys::math::C_Vector2 &);

        void SetZoomFlag(const bool);
    };
} // namespace SDK
