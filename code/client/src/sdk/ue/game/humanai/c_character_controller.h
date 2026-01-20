#pragma once

#include <cstdint>

#include <sdk/entities/c_actor.h>
#include <sdk/entities/e_actor_actions.h>
#include <sdk/entities/human/e_human_move_mode.h>
#include "c_character_state_handler.h"

#include "c_character_state_handler_aim.h"
#include "c_character_state_handler_car.h"
#include "c_character_state_handler_move.h"
#include "c_character_state_handler_weapon.h"

namespace SDK {
    class C_Human2;

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
