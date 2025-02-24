#pragma once

#include <cstdint>

#include "sdk/ue/sys/math/c_vector.h"

namespace SDK {
    class C_Entity;
    class C_HumanWeaponController {
      public:
        char pad0[0x158];                                   // 0000 - 0158
        ue::sys::math::C_Vector m_vAimDirection;            // 0158 - 0160 = Not safe as it's only filled up when the class belongs to a human (non player) owning entity
        int m_iAimFlag;                                     // 0160 - 0164
        char pad1[0x108];                                   // 0164 - 0270
        int *m_iRightHandWeaponID;                          // 0270 - 0278


      public:
        bool DoWeaponSelectByItemId(unsigned int, bool);

        int GetRightHandWeaponID() {
            if (m_iRightHandWeaponID) {
                return *m_iRightHandWeaponID;
            }
            return -1;
        }

        void GetAimDir(ue::sys::math::C_Vector *);
        void GetShotPosDir(ue::sys::math::C_Vector *, ue::sys::math::C_Vector *, float *);

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
