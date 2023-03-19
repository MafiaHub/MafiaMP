#pragma once

#include <cstdint>
#include "../../../entities/c_actor.h"
#include "c_character_state_handler.h"

#include "c_character_state_handler_aim.h"
#include "c_character_state_handler_car.h"
#include "c_character_state_handler_move.h"
#include "c_character_state_handler_weapon.h"

namespace SDK {
    class C_Human2;

    enum class E_HumanMoveMode {
        E_HMM_NONE   = -1,
        E_HMM_BREATH = 0,
        E_HMM_STEP,
        E_HMM_WALK_SLOW,
        E_HMM_WALK,
        E_HMM_WALK_FAST,
        E_HMM_RUN,
        E_HMM_SPRINT,
        E_HMM_AUTO_SLOW,
        E_HMM_AUTO_NORMAL,
        E_HMM_AUTO_FAST,
        E_HMM_END
    };

    enum E_ActorActions {
        E_AA_DUMMY                      = 0x0,
        E_AA_ENTER_CAR                  = 0x1,
        E_AA_LEAVE_CAR                  = 0x2,
        E_AA_OPEN_CAR_TRUNK             = 0x3,
        E_AA_CLOSE_CAR_TRUNK            = 0x4,
        E_AA_OPEN_CAR_HOOD              = 0x5,
        E_AA_CLOSE_CAR_HOOD             = 0x6,
        E_AA_TRAILER_MOUNT              = 0x7,
        E_AA_ITEM_PICK_UP               = 0x8,
        E_AA_USE_LADDER                 = 0x9,
        E_AA_SCRIPT                     = 0xA,
        E_AA_CUSTOMSCRIPT               = 0xB,
        E_AA_DOOR_OPEN                  = 0xC,
        E_AA_DOOR_KICK                  = 0xD,
        E_AA_DOOR_COMPONENT_OPEN        = 0xE,
        E_AA_DOOR_COMPONENT_PULL_LOCK   = 0xF,
        E_AA_DOOR_COMPONENT_KICK        = 0x10,
        E_AA_SAFEHOUSE_DOOR_ACCESS      = 0x11,
        E_AA_USE_LIFT                   = 0x12,
        E_AA_ACTOR_SIT                  = 0x13,
        E_AA_ACTOR_LEAVE                = 0x14,
        E_AA_RADIO                      = 0x15,
        E_AA_JUKEBOX                    = 0x16,
        E_AA_SLOT_MACHINE               = 0x17,
        E_AA_STEAL_CAR                  = 0x18,
        E_AA_STEAL_CAR_STOP             = 0x19,
        E_AA_BREAK_IN_CAR               = 0x1A,
        E_AA_LOCK_PICK_CAR              = 0x1B,
        E_AA_THROW_FROM_CAR             = 0x1C,
        E_AA_ENTER_BOAT                 = 0x1D,
        E_AA_LEAVE_BOAT                 = 0x1E,
        E_AA_RESPAWN_BOAT               = 0x1F,
        E_AA_ENTER_TRAIN                = 0x20,
        E_AA_LEAVE_TRAIN                = 0x21,
        E_AA_GET_ON_MOTORCYCLE          = 0x22,
        E_AA_GET_OFF_MOTORCYCLE         = 0x23,
        E_AA_THROW_FROM_MOTORCYCLE      = 0x24,
        E_AA_GARAGE_GET_CAR             = 0x25,
        E_AA_GARAGE_PARK_CAR            = 0x26,
        E_AA_GARAGE_SWITCH_CAR          = 0x27,
        E_AA_ENTER_INNERSPACE           = 0x28,
        E_AA_LEAVE_INNERSPACE           = 0x29,
        E_AA_USE_TELEPHONE              = 0x2A,
        E_AA_USE_STATIC_WEAPON          = 0x2B,
        E_AA_TV                         = 0x2C,
        E_AA_TAKE_PINUP                 = 0x2D,
        E_AA_DRAG_DEATH_BODY            = 0x2E,
        E_AA_STEALABLE_WEAPON           = 0x2F,
        E_AA_STEALABLE_SUIT             = 0x30,
        E_AA_STEALABLE_FOOD             = 0x31,
        E_AA_STEALABLE_AMMO             = 0x32,
        E_AA_OPEN_SHOP                  = 0x33,
        E_AA_USE_MOUNTED_WEAPON         = 0x34,
        E_AA_LEAVE_MOUNTED_WEAPON       = 0x35,
        E_AA_OPEN_HEADQUARTERS          = 0x36,
        E_AA_OPEN_CRIME_BUSINESS        = 0x37,
        E_AA_TELEPORT                   = 0x38,
        E_AA_BED                        = 0x39,
        E_AA_DEAD_GUY_THROWN_OUT_CAR    = 0x3A,
        E_AA_BAILOUT_CAR                = 0x3B,
        E_AA_BAILOUT_MOTORCYCLE         = 0x3C,
        E_AA_INTERROGATE_IN_CAR         = 0x3D,
        E_AA_PICKUP_SATCHEL             = 0x3E,
        E_AA_PICKUP_BODY                = 0x3F,
        E_AA_PUT_BODY_IN_CAR_TRUNK      = 0x40,
        E_AA_REMOVE_BODY_FROM_CAR_TRUNK = 0x41,
        E_AA_TURRET_MOUNT               = 0x42,
        E_AA_CAR_TURRET_MOUNT           = 0x43,
        E_AA_REVIVE                     = 0x44,
    };

    namespace ue::game::humanai {
        class I_CharacterController {
          public:
            virtual ~I_CharacterController() {}
            virtual uint64_t GetState(/*I_HumanActivityInspect::S_AdvancedState**/ uintptr_t) {
                return 0;
            } // GetHumanState
            virtual uint64_t GetMoveMode() {
                return 0;
            }
        };

        class C_CharacterController
            : public I_CharacterController
            , public I_EntityListener {
          protected:
            C_Human2 *m_pCharacter;
            uint8_t m_pad24[64];
            C_CharacterStateHandlerMove     *m_pMoveStateHandler;
            C_CharacterStateHandler         *m_pStandStateHandler;
            C_CharacterStateHandler         *m_pMoveStandStateHandler;
            C_CharacterStateHandler         *m_pCoverStateHandler;
            C_CharacterStateHandler         *m_pClimbStateHandler;
            C_CharacterStateHandler         *m_pActionAIStateHandler;
            C_CharacterStateHandler         *m_pInvestigateStateHandler;
            C_CharacterStateHandler         *m_pMeleeStateHandler;
            C_CharacterStateHandler         *m_pLockPullStateHandler;
            C_CharacterStateHandler         *m_pTurretStateHandler;
            C_CharacterStateHandler         *m_pPlayAnimStateHandler;
            C_CharacterStateHandler         *m_pDeathStateHandler;
            C_CharacterStateHandlerCar      *m_pCarStateHandler;
            C_CharacterStateHandler         *m_pBoatStateHandler;
            C_CharacterStateHandler         *m_pTrainStateHandler;
            C_CharacterStateHandler         *m_pMotorcycleStateHandler;
            C_CharacterStateHandler         *m_pAwarenessStateHandler;
            C_CharacterStateHandler         *m_pInjuryStateHandler;
            C_CharacterStateHandler         *m_pSpeechStateHandler;
            C_CharacterStateHandler         *m_pSwimStateHandler;
            C_CharacterStateHandler         *m_pPreviewStateHandler;
            C_CharacterStateHandler         *m_pFallStateHandler;
            C_CharacterStateHandler         *m_pCurrentStateHandler;
            C_CharacterStateHandlerWeapon   *m_pWeaponStateHandler;
            C_CharacterStateHandler         *m_pGrenadeStateHandler;
            C_CharacterStateHandlerAim      *m_pAimStateHandler;
            C_CharacterStateHandler         *m_pPlayOverlayAnimStateHandler;
            C_CharacterStateHandler         *m_pLookAtStateHandler;
            C_CharacterStateHandler         *m_pDownedStateHandler;
            C_CharacterStateHandler         *m_pPlayerReactionStateHandler;
            C_CharacterStateHandler         *m_pActionOverlayStateHandler;
            uintptr_t m_pCharacterLocomotionController;
            uint8_t m_pad344[96];
            S_PlayerControllerInput *m_pPlayerControllerInput;
            uintptr_t m_pPlayerState;
            uintptr_t m_pUnk456;
            float m_SprintMoveSpeed;
            bool m_IsSprinting;
            bool m_Unk469;
            bool m_Unk470;
            bool m_Unk471;
            int m_LockControlsCount;
            uint8_t m_pad476[24];
            bool m_InAimZoom;
            uint8_t m_pad501[123];
            uintptr_t m_pPlayerLocomotion;
            uint8_t m_pad632[216];

          public:
            virtual ~C_CharacterController() {}

            C_Human2 *GetCharacter() {
                return m_pCharacter;
            }
            C_CharacterStateHandlerAim* GetAimHandler() {
                return m_pAimStateHandler;
            }
            C_CharacterStateHandlerCar *GetCarHandler() {
                return m_pCarStateHandler;
            }
            C_CharacterStateHandler *GetCurrentStateHandler() {
                return m_pCurrentStateHandler;
            }
            C_CharacterStateHandler::E_State_Handler_Type GetCurrentStateHandlerType() const;
            C_CharacterStateHandler *GetHandlerByType(C_CharacterStateHandler::E_State_Handler_Type type);
            uintptr_t GetCharacterLocomotionController() {
                return m_pCharacterLocomotionController;
            }
            S_PlayerControllerInput *GetPlayerControllerInput() {
                return m_pPlayerControllerInput;
            }
            uintptr_t GetPlayerState() {
                return m_pPlayerState;
            }
            bool AreControlsLocked() const;
            void LockControls(bool a2 = false, bool a3 = false, const char *a4 = "");
            void UnlockControls(const char *a2 = "");
            float GetSprintMoveSpeed() const {
                return m_SprintMoveSpeed;
            }
            bool IsSprinting() const {
                return m_IsSprinting;
            }
            void SetSprinting(bool isSprinting) {
                m_IsSprinting = isSprinting;
            }
            bool IsInAimZoom() const {
                return m_InAimZoom;
            };
            uintptr_t GetPlayerLocomotion() {
                return m_pPlayerLocomotion;
            }

            void *ActivateHandler(C_CharacterStateHandler *, bool);
            void DeactivateHandler_FromPlayerInput();
            E_HumanMoveMode GetHumanMoveMode();
            void SetHumanMoveMode(E_HumanMoveMode, bool clearUnkFlagIfSprint = false);
            bool IsStalkMove();
            void SetStalkMove(bool);
            void SetSprintMoveSpeed(float sprintMoveSpeed) {
                m_SprintMoveSpeed = sprintMoveSpeed;
            }
            bool TriggerActorAction(C_Actor *, E_ActorActions, int, bool, bool);
        };
        static_assert(sizeof(C_CharacterController) == 848, "Wrong size: ue::game::humanai::C_CharacterController");
    } // namespace ue::game::humanai
}
