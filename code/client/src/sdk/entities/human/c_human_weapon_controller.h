#pragma once

#include <cstdint>

#include "../../ue/sys/math/c_vector.h"

namespace SDK {
    class C_Entity;
    class C_HumanWeaponController {
      public:
        char pad0[0x270];                   // 0000 - 0270
        int *m_iRightHandWeaponID;          // 0270 - 0278


      public:
        bool DoWeaponSelectByItemId(unsigned int, bool);

        int GetRightHandWeaponID() {
            if (m_iRightHandWeaponID) {
                return *m_iRightHandWeaponID;
            }
            return -1;
        }

        void GetShotPosDir(SDK::ue::sys::math::C_Vector *, SDK::ue::sys::math::C_Vector *, float *);

        bool IsThrownWeapon();

        void ResetScatterCoef();

        void SetAiming(const bool);
        bool IsAiming();

        void SetCoverFlag(const bool);

        void SetFirePressedFlag(const bool);

        bool DoShot(void *, ue::sys::math::C_Vector *, ue::sys::math::C_Vector *, bool, bool);
        bool DoWeaponReloadInventory(int);

        // TODO: Crashes, could it be that I don't have a gamepad? Maybe pattern is wrong?
        void SetStickMove(const ue::sys::math::C_Vector2 &);

        void SetZoomFlag(const bool);
    };
} // namespace SDK
