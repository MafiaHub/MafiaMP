#pragma once

#include "c_character_state_handler_base_vehicle.h"

namespace SDK {
    class C_Human2CarWrapper;
    namespace ue::game::humanai {
        // Size: ~624 bytes (0x270)
        class C_CharacterStateHandlerCar : public C_CharacterStateHandlerBaseVehicle {
          public:
            enum E_CAR_STATE {
                E_CS_NONE             = 0,
                E_CS_GETTING_IN       = 1,
                E_CS_ENTERING         = 2,
                E_CS_DRIVING          = 3,
                E_CS_IDLE             = 4,
                E_CS_PRE_LEAVING      = 5,
                E_CS_OPEN_DOOR        = 6,
                E_CS_GETTING_OUT      = 7,
                E_CS_FAST_EXIT        = 8,
                E_CS_SWITCH_SEAT      = 9,
                E_CS_HOTWIRE_OPEN     = 10,
                E_CS_HOTWIRE_CLOSE    = 11,
                E_CS_LEAVE_START      = 12,
                E_CS_LEAVE            = 13,
                E_CS_SWITCH_SIDE      = 14,
                E_CS_BAILOUT          = 15,
                E_CS_THROW_OUT        = 16,
            };

          protected:
            uintptr_t m_WrapperHeader;                     // 0x180 - 0x188
            C_Human2CarWrapper *m_pCarWrapper;             // 0x188 - 0x190
            uintptr_t m_pSeatInfo;                         // 0x190 - 0x198
            uint32_t m_nCarFlags;                          // 0x198 - 0x19C
            int32_t m_nCurrentSeatIndex;                   // 0x19C - 0x1A0 (default -1)
            int32_t m_nPrevCarState;                       // 0x1A0 - 0x1A4
            E_CAR_STATE m_CarState;                        // 0x1A4 - 0x1A8
            uintptr_t m_TaskQueueBegin;                    // 0x1A8 - 0x1B0
            uintptr_t m_TaskQueueEnd;                      // 0x1B0 - 0x1B8
            uintptr_t m_TaskQueueCapacity;                 // 0x1B8 - 0x1C0
            uintptr_t m_unk448;                            // 0x1C0 - 0x1C8
            uintptr_t m_unk456;                            // 0x1C8 - 0x1D0
            bool m_bCanSwitchWeapon;                       // 0x1D0 - 0x1D1
            char _pad0[0x7];                               // 0x1D1 - 0x1D8
            uintptr_t m_unk472;                            // 0x1D8 - 0x1E0
            void *m_pDLAMMRefPtr;                          // 0x1E0 - 0x1E8
            bool m_bUnk488;                                // 0x1E8 - 0x1E9
            char _pad1[0x7];                               // 0x1E9 - 0x1F0
            uintptr_t m_unk496;                            // 0x1F0 - 0x1F8
            float m_fSteeringLeft;                         // 0x1F8 - 0x1FC (default -1.0f)
            float m_fSteeringRight;                        // 0x1FC - 0x200 (default -1.0f)
            int32_t m_nGearChangeFlags;                    // 0x200 - 0x204 (default 256)
            bool m_bUnk516;                                // 0x204 - 0x205
            char _pad2[0x3];                               // 0x205 - 0x208
            uintptr_t m_unk520;                            // 0x208 - 0x210
            uintptr_t m_unk528;                            // 0x210 - 0x218
            uintptr_t m_unk536;                            // 0x218 - 0x220
            uintptr_t m_pLeaveCarData;                     // 0x220 - 0x228
            int32_t m_nLeaveCarFlags;                      // 0x228 - 0x22C
            int32_t m_nTargetSwitchSeat;                   // 0x22C - 0x230 (default -1)
            uintptr_t m_unk560;                            // 0x230 - 0x238
            uintptr_t m_pLeaveCarAnim;                     // 0x238 - 0x240
            int32_t m_nExitFlags;                          // 0x240 - 0x244
            float m_fExitSpeed;                            // 0x244 - 0x248 (default -1.0f)
            bool m_bRagdollOnExit;                         // 0x248 - 0x249
            char _pad3[0x3];                               // 0x249 - 0x24C
            int32_t m_nExitState;                          // 0x24C - 0x250
            uintptr_t m_unk592;                            // 0x250 - 0x258
            uintptr_t m_unk600;                            // 0x258 - 0x260
            uintptr_t m_unk608;                            // 0x260 - 0x268
            uintptr_t m_unk616;                            // 0x268 - 0x270

          public:
            E_CAR_STATE GetCarState() const {
                return m_CarState;
            }

            C_Human2CarWrapper *GetHuman2CarWrapper() {
                return m_pCarWrapper;
            }

            int32_t GetCurrentSeatIndex() const {
                return m_nCurrentSeatIndex;
            }

            bool CanSwitchWeapon() const {
                return m_bCanSwitchWeapon;
            }

            void GoToCarState(E_CAR_STATE state, bool forceChange = false);
            void AddTask(int taskType, int seatIndex, bool immediate);
            void UpdateCarTasks();
            void SetupEnterCar();
            void SetupSwitchSeat(int targetSeat);
            void EnableWeaponSwitch(bool enable);
            void EnableWeaponSwitchAI(bool enable);
            void PerformLeaveCarStuff();
            void EndAIRequest();
            void CheckChangeGears();
            bool DrivingReverse() const;
            void UpdateDLAMMFinish();
            void SetupDelayedPlayerActivation();
        };
    } // namespace ue::game::humanai
} // namespace SDK
