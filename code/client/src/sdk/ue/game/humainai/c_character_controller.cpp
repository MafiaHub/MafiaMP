#include "c_character_controller.h"

#include "c_character_state_handler_base_locomotion.h"

#include "../../../entities/c_player_2.h"
#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::humanai {
        C_CharacterStateHandler::E_State_Handler_Type C_CharacterController::GetCurrentStateHandlerType() const {
            if (m_pCurrentStateHandler) {
                return m_pCurrentStateHandler->GetStateHandlerType();
            }
            return C_CharacterStateHandler::E_SHT_NONE;
        }

        C_CharacterStateHandler *C_CharacterController::GetHandlerByType(C_CharacterStateHandler::E_State_Handler_Type type) {
            switch (type) {
            case C_CharacterStateHandler::E_SHT_DEATH: return m_pDeathStateHandler;
            case C_CharacterStateHandler::E_SHT_MOVE: return m_pMoveStateHandler;
            case C_CharacterStateHandler::E_SHT_STAND: return m_pStandStateHandler;
            case C_CharacterStateHandler::E_SHT_MOVESTAND: return m_pMoveStandStateHandler;
            case C_CharacterStateHandler::E_SHT_CLIMB: return m_pClimbStateHandler;
            case C_CharacterStateHandler::E_SHT_COVER: return m_pCoverStateHandler;
            case C_CharacterStateHandler::E_SHT_CAR: return m_pCarStateHandler;
            case C_CharacterStateHandler::E_SHT_MELEE: return m_pMeleeStateHandler;
            case C_CharacterStateHandler::E_SHT_ACTION: return m_pActionAIStateHandler;
            case C_CharacterStateHandler::E_SHT_WEAPON: return m_pWeaponStateHandler;
            case C_CharacterStateHandler::E_SHT_GRENADE: return m_pGrenadeStateHandler;
            case C_CharacterStateHandler::E_SHT_AIM: return m_pAimStateHandler;
            case C_CharacterStateHandler::E_SHT_AWARENESS: return m_pAwarenessStateHandler;
            case C_CharacterStateHandler::E_SHT_INJURY: return m_pInjuryStateHandler;
            case C_CharacterStateHandler::E_SHT_SPEECH: return m_pSpeechStateHandler;
            case C_CharacterStateHandler::E_SHT_PLAYANIM: return m_pPlayAnimStateHandler;
            case C_CharacterStateHandler::E_SHT_PLAYOVERLAYANIM: return m_pPlayOverlayAnimStateHandler;
            case C_CharacterStateHandler::E_SHT_LOOKAT: return m_pLookAtStateHandler;
            case C_CharacterStateHandler::E_SHT_SWIM: return m_pSwimStateHandler;
            case C_CharacterStateHandler::E_SHT_BOAT: return m_pBoatStateHandler;
            case C_CharacterStateHandler::E_SHT_PREVIEW: return m_pPreviewStateHandler;
            case C_CharacterStateHandler::E_SHT_FALL: return m_pFallStateHandler;
            case C_CharacterStateHandler::E_SHT_LOCKPULL: return m_pLockPullStateHandler;
            case C_CharacterStateHandler::E_SHT_PLAYERREACTION: return m_pPlayerReactionStateHandler;
            case C_CharacterStateHandler::E_SHT_ACTION_OVERLAY: return m_pActionOverlayStateHandler;
            case C_CharacterStateHandler::E_SHT_TRAIN: return m_pTrainStateHandler;
            case C_CharacterStateHandler::E_SHT_MOTORCYCLE: return m_pMotorcycleStateHandler;
            case C_CharacterStateHandler::E_SHT_DOWNED: return m_pDownedStateHandler;
            case C_CharacterStateHandler::E_SHT_TURRET: return m_pTurretStateHandler;
            case C_CharacterStateHandler::E_SHT_INVESTIGATE: return m_pInvestigateStateHandler;
            default: return nullptr;
            }
        }

        bool C_CharacterController::AreControlsLocked() const {
            if (m_pCharacter->GetType() == E_EntityType::E_ENTITY_PLAYER) {
                return (*(uint32_t *)(m_pUnk456 + 64) >> 19) & 1;
            }
            return false;
        }

        void C_CharacterController::LockControls(bool, bool, const char *) {
            if (m_pCharacter->GetType() == E_EntityType::E_ENTITY_PLAYER) {
                ((C_Player2 *)m_pCharacter)->LockControls(true);
                ++m_LockControlsCount;
            }
        }

        void C_CharacterController::UnlockControls(const char *) {
            if (m_pCharacter->GetType() == E_EntityType::E_ENTITY_PLAYER && --m_LockControlsCount <= 0) {
                ((C_Player2 *)m_pCharacter)->LockControls(false);
                m_LockControlsCount = 0;
            }
        }

        void *C_CharacterController::ActivateHandler(ue::game::humanai::C_CharacterStateHandler *pStateHandler, bool unk) {
            return hook::this_call<void *>(gPatterns.C_CharacterController__ActivateHandler, this, pStateHandler, unk);
        }

        void C_CharacterController::DeactivateHandler_FromPlayerInput() {
            hook::this_call<bool>(gPatterns.C_CharacterController__DeactivateHandler_FromPlayerInput, this);
        }

        E_HumanMoveMode C_CharacterController::GetHumanMoveMode() {
            return *(E_HumanMoveMode *)(m_pUnk456 + 100);
        }

        void C_CharacterController::SetHumanMoveMode(E_HumanMoveMode mode, bool clearUnkFlagIfSprint) {
            *(E_HumanMoveMode *)(m_pUnk456 + 100) = mode;

            if (mode == E_HumanMoveMode::E_HMM_SPRINT && clearUnkFlagIfSprint) {
                *(uint32_t *)(m_pUnk456 + 64) &= 0xEFFFFFFF;
            }
        }

        bool C_CharacterController::IsStalkMove() {
            return (*(uint32_t *)(m_pUnk456 + 64) >> 28) & 1;
        }

        void C_CharacterController::SetStalkMove(bool stalkMove) {
            if (m_pCharacter->GetType() == E_EntityType::E_ENTITY_PLAYER) {
                bool previousState = IsStalkMove();
                *(uint32_t *)(m_pUnk456 + 64) &= 0xEFFFFFFF;
                *(uint32_t *)(m_pUnk456 + 64) |= (stalkMove & 1) << 28;
                if (stalkMove != previousState) {
                    if (m_IsSprinting) {
                        m_IsSprinting = false;
                        C_CharacterStateHandlerBaseLocomotion::AddRemoveSprintDescriptor(this, false);
                    }
                }
            }
            else {
                *(uint32_t *)(m_pUnk456 + 64) &= 0xDFFFFFFF;
                *(uint32_t *)(m_pUnk456 + 64) |= (stalkMove & 1) << 29;
            }
        }

        bool C_CharacterController::TriggerActorAction(C_Actor *pActor, E_ActorActions actorAction, int a4, bool a5, bool a6) {
            return hook::this_call<bool>(gPatterns.C_CharacterController__TriggerActorActionById, this, pActor, actorAction, a4, a5, a6);
        }
    } // namespace ue::game::humanai
} // namespace SDK
