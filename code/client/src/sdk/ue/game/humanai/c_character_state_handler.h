#pragma once

#include "../human/c_behavior_character.h"
#include "../anim/s_wanim_state_handle.h"
#include "../../sys/math/c_matrix.h"
#include "../../c_ref_ptr.h"

#include "s_player_controller_input.h"

namespace SDK {
    class C_Human2;
    namespace ue::game::humanai {
        class C_CharacterController;
        class C_CharacterStateHandler {
          public:
            enum E_State_Handler_Type {
                E_SHT_NONE,
                E_SHT_DEATH,
                E_SHT_MOVE,
                E_SHT_STAND,
                E_SHT_MOVESTAND,
                E_SHT_CLIMB,
                E_SHT_COVER,
                E_SHT_CAR,
                E_SHT_MELEE,
                E_SHT_ACTION,
                E_SHT_WEAPON,
                E_SHT_GRENADE,
                E_SHT_AIM,
                E_SHT_AWARENESS,
                E_SHT_INJURY,
                E_SHT_SPEECH,
                E_SHT_PLAYANIM,
                E_SHT_PLAYOVERLAYANIM,
                E_SHT_LOOKAT,
                E_SHT_SWIM,
                E_SHT_BOAT,
                E_SHT_PREVIEW,
                E_SHT_FALL,
                E_SHT_LOCKPULL,
                E_SHT_PLAYERREACTION,
                E_SHT_ACTION_OVERLAY,
                E_SHT_TRAIN,
                E_SHT_MOTORCYCLE,
                E_SHT_DOWNED,
                E_SHT_TURRET,
                E_SHT_INVESTIGATE,
                E_SHT_LAST
            };

            static const char *GetStateHandlerTypeName(E_State_Handler_Type type);
            static bool IsVehicleStateHandlerType(E_State_Handler_Type type) {
                return (type == E_SHT_CAR || type == E_SHT_BOAT || type == E_SHT_TRAIN || type == E_SHT_MOTORCYCLE);
            }

          protected:
            ue::game::human::C_BehaviorCharacter *m_pBehaviorCharacter;
            C_CharacterController *m_pController;
            uintptr_t m_unk24;
            uint32_t m_LocomotionState;
            uint32_t m_unk36;
            C_Human2 *m_pCharacter;
            E_State_Handler_Type m_Type;
            uint32_t m_unk52;
            ue::game::anim::S_WAnimStateHandle m_CurrentAnimationHandle;

          public:
            virtual ~C_CharacterStateHandler()                                                                                                = 0;
            virtual bool Activate(/*ue::C_WeakPtr<game::ai::S_HumanAIRequest,ue::S_SingleThreadedPolicy,0>*/ void *) = 0;
            virtual void Deactivate()                                                                                                         = 0;
            virtual void UpdateRequest(/*ue::C_WeakPtr<game::ai::S_HumanAIRequest,ue::S_SingleThreadedPolicy,0>*/ void *) = 0;
            virtual void UpdateAIFreq(/*ue::C_WeakPtr<game::ai::S_HumanAIRequest,ue::S_SingleThreadedPolicy,0>*/ void *) = 0;
            virtual bool UpdateHumanFreq(float)                                                                                               = 0;
            virtual void UpdateHumanFreqPostPhysics(float)                                                                                    = 0;
            virtual void UpdateHumanFreqPostPhysicsCamera(float)                                                                              = 0;
            virtual void Start()                                                                                                              = 0;
            virtual void Finish(C_CharacterStateHandler *)                                                                                    = 0;
            virtual void ForceSuccess(/*ue::C_WeakPtr<game::ai::S_HumanAIRequest,ue::S_SingleThreadedPolicy,0>*/ void *) = 0;
            virtual bool CanDeactivatedByHandler(C_CharacterStateHandler *)                                                                   = 0;
            virtual void AcceptMessage(/*ue::game::humanai::S_CharacterMessage const&*/ void *) = 0;
            virtual bool ReqPlayerInputActivate(ue::game::humanai::S_PlayerControllerInput *, /*ue::game::humanai::S_PlayerState**/ void *)   = 0;
            virtual bool ReqPlayerInputDeactivate(ue::game::humanai::S_PlayerControllerInput *, /*ue::game::humanai::S_PlayerState**/ void *) = 0;
            virtual void PreUpdatePlayerInput(float)                                                                                          = 0;
            virtual void UpdatePlayerInput(ue::game::humanai::S_PlayerControllerInput *, /*ue::game::humanai::S_PlayerState**/ void *);
            virtual void OnCharacterControllerActivated()                                                                                                                                                            = 0;
            virtual void OnCharacterControllerDeactivated()                                                                                                                                                          = 0;
            virtual void ProcessRootMotion(ue::sys::math::C_Matrix const &, ue::sys::math::C_Matrix &, float)                                                                                                        = 0;
            virtual void ProcessRootMotionWithDocking(ue::sys::math::C_Matrix const &, ue::sys::math::C_Matrix &, float)                                                                                             = 0;
            virtual void ReportRootMotion(ue::sys::math::C_Matrix const &, ue::sys::math::C_Matrix const &, float)                                                                                                   = 0;
            virtual void OnSetPosFeedBack(ue::sys::math::C_Vector const &, ue::sys::math::C_Vector const &)                                                                                                          = 0;
            virtual bool CanEnterAimZoom()                                                                                                                                                                           = 0;
            virtual bool CanEnterAimZoomModeScope()                                                                                                                                                                  = 0;
            virtual bool CanFire()                                                                                                                                                                                   = 0;
            virtual bool ShouldEnterAimZoomModeScope()                                                                                                                                                               = 0;
            virtual bool CanThrowGrenade()                                                                                                                                                                           = 0;
            virtual bool CanStartBlindFire()                                                                                                                                                                         = 0;
            virtual void OnEnterAimZoom()                                                                                                                                                                            = 0;
            virtual void OnExitAimZoom()                                                                                                                                                                             = 0;
            virtual void OnEnterTakedown()                                                                                                                                                                           = 0;
            virtual void OnExitTakedown()                                                                                                                                                                            = 0;
            virtual void OnStartBlindFire()                                                                                                                                                                          = 0;
            virtual void OnUpdateBlindFire(float)                                                                                                                                                                    = 0;
            virtual void OnStopBlindFire()                                                                                                                                                                           = 0;
            virtual void OnInterruptible(ue::game::anim::S_WAnimStateHandle const &, unsigned int)                                                                                                                   = 0;
            virtual void OnCameraModeChangeRequest(ue::game::anim::S_WAnimStateHandle const &, ue::sys::utils::C_HashName const &, bool)                                                                             = 0;
            virtual void OnCameraModeChangeRestore(ue::game::anim::S_WAnimStateHandle const &, bool)                                                                                                                 = 0;
            virtual void OnCameraFOVChange(ue::game::anim::S_WAnimStateHandle const &, float, float)                                                                                                                 = 0;
            virtual void OnCameraInterruptible(ue::game::anim::S_WAnimStateHandle const &, bool, float)                                                                                                              = 0;
            virtual void OnCameraLookAt(ue::game::anim::S_WAnimStateHandle const &, /*ue::game::human::E_MeleeHumanType*/ int, ue::sys::utils::C_HashName const &, ue::sys::utils::C_HashName const &, float, float) = 0;
            virtual void OnCameraFollow(ue::game::anim::S_WAnimStateHandle const &, ue::sys::utils::C_HashName const &, float, float, float)                                                                         = 0;
            virtual void OnCameraStopFollowLookAt(ue::game::anim::S_WAnimStateHandle const &)                                                                                                                        = 0;
            virtual void OnCameraShake(ue::game::anim::S_WAnimStateHandle const &, ue::sys::utils::C_HashName const &, float)                                                                                        = 0;

          public:
            ue::game::human::C_BehaviorCharacter *GetBehaviorCharacter() const {
                return m_pBehaviorCharacter;
            }
            C_CharacterController *GetController() const {
                return m_pController;
            }
            uint32_t GetLocomotionState() const {
                return m_LocomotionState;
            }
            C_Human2 *GetCharacter() const {
                return m_pCharacter;
            }
            E_State_Handler_Type GetStateHandlerType() const {
                return m_Type;
            }
            ue::game::anim::S_WAnimStateHandle &GetCurrentAnimationHandle() {
                return m_CurrentAnimationHandle;
            }
            ue::game::anim::S_WAnimStateHandle *GetCurrentAnimationHandlePtr() {
                return &m_CurrentAnimationHandle;
            }
            void SetUnk52(uint32_t unk52) {
                m_unk52 = unk52;
            }
            void SetCurrentAnimationHandle(ue::game::anim::S_WAnimStateHandle &&handle) {
                m_CurrentAnimationHandle = handle;
            }
        };
    } // namespace ue::game::humanai
}
