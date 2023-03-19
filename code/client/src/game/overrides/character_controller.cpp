#include "character_controller.h"

#include "../../sdk/c_ie.h"

#include "scoped_entity_type_faker.h"

#include <logging/logger.h>

namespace MafiaMP::Game::Overrides {
    uintptr_t CharacterController::_vfTables[2]            = {0, 0};
    static uintptr_t C_CharacterController__DestructorAddr  = 0;
    static uintptr_t C_CharacterController__GetStateAddr    = 0;
    static uintptr_t C_CharacterController__GetMoveModeAddr = 0;
    static uintptr_t C_CharacterController__OnEventAddr     = 0;

    CharacterController::CharacterController() {
        if (!_vfTables[0] && !_vfTables[1]) {
            _vfTables[0]  = ((uintptr_t *)this)[0];
            _vfTables[1] = ((uintptr_t *)this)[1];
        }

        static char DummyMoveStandStateHandler[4096]; // fake shit
        m_pMoveStandStateHandler = (decltype(m_pMoveStandStateHandler))DummyMoveStandStateHandler;

        m_pCharacterLocomotionController                                   = (uintptr_t)SDK::C_IE::Alloc(256);
        static const auto C_CharacterLocomotionController__ConstructorAddr = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC 20 33 C0 48 89 11 48 89 41 08 48 8B D9").get_first());
        hook::this_call<void>(C_CharacterLocomotionController__ConstructorAddr, m_pCharacterLocomotionController, this);

        *(bool *)((uint8_t *)(m_pMeleeStateHandler) + 2644) = false;
    }

    CharacterController::~CharacterController() {
        m_pMoveStandStateHandler = nullptr;
        hook::this_call<void>(C_CharacterController__DestructorAddr, this, false);
    }

    CharacterController *CharacterController::Create(OrigConstructor_t OrigConstructor, SDK::C_Human2 *pHuman2) {
        ScopedEntityTypeFaker faker(pHuman2, SDK::E_EntityType::E_ENTITY_PLAYER);
        CharacterController *pCharCtrler = (CharacterController *)SDK::C_IE::Alloc(sizeof(CharacterController));
        OrigConstructor(pCharCtrler, pHuman2);

        if (!C_CharacterController__DestructorAddr) {
            uintptr_t CharCtrlerVFTable            = ((uintptr_t *)pCharCtrler)[0];
            uintptr_t EntityListenerVFTable        = ((uintptr_t *)pCharCtrler)[1];
            C_CharacterController__DestructorAddr  = ((uintptr_t *)CharCtrlerVFTable)[0];
            C_CharacterController__GetStateAddr    = ((uintptr_t *)CharCtrlerVFTable)[1];
            C_CharacterController__GetMoveModeAddr = ((uintptr_t *)CharCtrlerVFTable)[2];
            C_CharacterController__OnEventAddr     = ((uintptr_t *)EntityListenerVFTable)[1];
        }

        return new (pCharCtrler) CharacterController();
    }

    void CharacterController::operator delete(void *ptr) {
        SDK::C_IE::Free(ptr);
    }

    bool CharacterController::IsInstanceOfClass(void *pInstance) {
        return (*(uintptr_t *)pInstance == _vfTables[0] && ((uintptr_t *)pInstance)[1] == _vfTables[1]);
    }

    uint64_t CharacterController::GetState(uintptr_t a2) {
        return hook::this_call<uint64_t>(C_CharacterController__GetStateAddr, this, a2);
    }

    uint64_t CharacterController::GetMoveMode() {
        return hook::this_call<uint64_t>(C_CharacterController__GetMoveModeAddr, this);
    }

    void CharacterController::OnEvent(SDK::C_Entity *a2, int a3) {
        hook::this_call<void>(C_CharacterController__OnEventAddr, this, a2, a3);
    }

    void CharacterController::ActivateVehicleStateHandler(SDK::ue::game::humanai::C_CharacterStateHandler *pNewHandler, bool unk) {
        ScopedEntityTypeFaker restore(m_pCharacter, SDK::E_EntityType::E_ENTITY_HUMAN);
        pNewHandler->SetUnk52(1);
        m_Unk470 = unk;
        if (m_pCurrentStateHandler != pNewHandler) {
            m_pCurrentStateHandler  = pNewHandler;
            static const auto unkfn = hook::get_opcode_address("E8 ? ? ? ? EB 10 48 8B CF");
            hook::this_call<void>(unkfn, this);
        }
        pNewHandler->Start();
    }

    void CharacterController::SwitchHandler(SDK::ue::game::humanai::C_CharacterStateHandler::E_State_Handler_Type newHandlerType) {
        SDK::ue::game::humanai::C_CharacterStateHandler::E_State_Handler_Type CurrentStateHandlerType = m_pCurrentStateHandler->GetStateHandlerType();
        if (CurrentStateHandlerType == newHandlerType) {
            return;
        }

        SDK::ue::game::humanai::C_CharacterStateHandler *pNewHandler = GetHandlerByType(newHandlerType);
        if (!pNewHandler || m_pCurrentStateHandler == pNewHandler) {
            return;
        }

        if (!SDK::ue::game::humanai::C_CharacterStateHandler::IsVehicleStateHandlerType(CurrentStateHandlerType)) {
            ScopedEntityTypeFaker faker(GetCharacter(), SDK::E_EntityType::E_ENTITY_PLAYER);
            DeactivateHandler_FromPlayerInput();
        }
        else {
            ScopedEntityTypeFaker restore(GetCharacter(), SDK::E_EntityType::E_ENTITY_HUMAN);
            DeactivateHandler_FromPlayerInput();
        }
        ActivateHandler(pNewHandler, false);
    }

    void CharacterController::OnUpdateLocomotionHandlers(float) {
        ScopedEntityTypeFaker faker(m_pCharacter, SDK::E_EntityType::E_ENTITY_PLAYER);

        if (_desiredHandlerType == SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_NONE) {
            return;
        }

        if (!SDK::ue::game::humanai::C_CharacterStateHandler::IsVehicleStateHandlerType(_desiredHandlerType)) {
            // If the character is actually falling, we block the game trying to override anything
            if (m_pCurrentStateHandler->GetStateHandlerType() == SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_FALL) {
                uintptr_t _840 = *(uintptr_t *)(((uintptr_t)m_pCharacter) + 840);
                if (*(uint8_t *)(*(uintptr_t *)(_840 + 664) + 300) & 1 && !*(uint8_t *)(((uintptr_t)m_pCharacter) + 505)) {
                    return;
                }
            }

            SwitchHandler(_desiredHandlerType);
        }

        SDK::ue::game::humanai::C_CharacterStateHandler::E_State_Handler_Type CurrentStateHandlerType = m_pCurrentStateHandler->GetStateHandlerType();
        if (CurrentStateHandlerType != _desiredHandlerType) {
            return;
        }

        SetStalkMove(_stalkMoveOverride);
        if (_stalkMoveOverride) {
            _isSprintingOverride = false;
        }

        if (_desiredHandlerType == SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_MOVE) {
            SDK::ue::game::humanai::C_CharacterStateHandlerMove *pMoveHandler = (SDK::ue::game::humanai::C_CharacterStateHandlerMove *)m_pCurrentStateHandler;

            if (_humanMoveMode != GetHumanMoveMode()) {
                SetHumanMoveMode(_humanMoveMode, true);
            }

            SetSprintMoveSpeed(_sprintMoveSpeed);

            if (IsSprinting() != _isSprintingOverride) {
                SetSprinting(_isSprintingOverride);
                SDK::ue::game::humanai::C_CharacterStateHandlerBaseLocomotion::AddRemoveSprintDescriptor(this, _isSprintingOverride);
                if (!pMoveHandler->Idle2MoveTransitionActive(pMoveHandler->GetCurrentAnimationHandlePtr()) && !pMoveHandler->SharpTurnTransitionActive()) {
                    static SDK::ue::sys::utils::C_HashName s_MoveState             = SDK::ue::sys::utils::C_HashName::ComputeHash("Move");
                    static SDK::ue::sys::utils::C_HashName s_SprintState           = SDK::ue::sys::utils::C_HashName::ComputeHash("Sprint");
                    SDK::ue::sys::utils::C_HashName state                          = pMoveHandler->IsSprinting() ? s_SprintState : s_MoveState;
                    SDK::ue::game::anim::C_WAnimPlaybackManager *pWAnimPlaybackMgr = pMoveHandler->GetBehaviorCharacter()->GetWAnimPlaybackManager();
                    if (pWAnimPlaybackMgr)
                        pMoveHandler->SetCurrentAnimationHandle(pWAnimPlaybackMgr->PlayState(state, 0, -1.0f, -1));
                    else
                        pMoveHandler->SetCurrentAnimationHandle(SDK::ue::game::anim::S_WAnimStateHandle(0));
                }
            }

            float depthInWater = m_pCharacter->GetDepthInWater();
            if (depthInWater > 0.0f) {
                pMoveHandler->GetBehaviorCharacter()->SetWAnimVariable(21, depthInWater * 100.0f);
            }
        } else {
            SetSprinting(false);
        }
    }
}
