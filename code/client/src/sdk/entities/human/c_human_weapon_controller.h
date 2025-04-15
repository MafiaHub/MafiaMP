#pragma once

#include <cstdint>

#include "sdk/ue/sys/math/c_vector.h"

namespace SDK {
    class C_Entity;
    class C_HumanWeaponController {
      public:
        char pad0[0x90];                                    // 0000 - 0090
        uint32_t m_dwFirePressStartTime;                    // 0x90 - 0x94: Timestamp/counter when fire was first pressed.
        bool m_bFirePressed;                                // 0x94 - 0x95: Current fire button state.
        bool m_bFireHeld;                                   // 0x95 - 0x96: Indicates if the fire button is held.
        uint16_t m_padFire;                                 // 0x96 - 0x98: Unk - Padding
        uint32_t m_dwFireHoldDuration;                      // 0x98 - 0x9C: Accumulated duration while the button is held.
        char pad1[0xBC];                                    // 0x9C - 0x158
        ue::sys::math::C_Vector m_vAimDirection;            // 0158 - 0160: Not safe as it's only filled up when the class belongs to a human (non player) owning entity
        int m_iAimFlag;                                     // 0160 - 0164
        char pad2[0x108];                                   // 0164 - 0270
        int *m_iRightHandWeaponID;                          // 0270 - 0278
        char pad3[0x104];                                   // 0278 - 037C
        uint8_t m_uAimingFlag;                              // 037C - 037D


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
