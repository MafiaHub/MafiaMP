#pragma once

#include "sdk/ai/sight/c_human_detector.h"
#include "sdk/c_corpse_emitter.h"
#include "sdk/c_human_emitter.h"
#include "sdk/c_melee_emitter.h"
#include "sdk/c_weapon_emitter.h"
#include "sdk/game/ai/hear/c_actorear.h"
#include "sdk/game/ai/sight/c_actor_emitter.h"
#include "sdk/mafia/health_system/c_health_system.h"
#include "sdk/ue/c_ptr.h"
#include "sdk/ue/game/audio/radio/c_radio_sound.h"
#include "sdk/ue/game/humainai/c_character_controller.h"
#include "sdk/ue/game/humainai/c_character_state.h"
#include "sdk/ue/game/human/c_agent.h"
#include "sdk/ue/game/human/c_behavior_character.h"
#include "sdk/ue/game/injury/c_injuryanalyser.h"
#include "sdk/ue/sys/core/c_model_component.h"
#include "sdk/ue/sys/core/c_scene_object.h"
#include "sdk/ue/sys/math/c_matrix.h"
#include "sdk/ue/sys/math/c_vector.h"
#include "sdk/ue/sys/utils/c_hash_name.h"
#include "c_actor.h"
#include "human/c_human_ai_controller.h"
#include "human/c_human_head_controller.h"
#include "human/c_human_inventory.h"
#include "human/c_human_script.h"
#include "human/c_human_weapon_controller.h"
#include "c_entity_ownership_human_car.h"
#include "sdk/c_inventory_wrapper.h"

#include <cstdint>

namespace SDK {
    class I_Human2: public C_Actor {
        friend class C_HumanHeadController;
        friend class C_HumanScript;

      public:
        char pad0[0x100];                                                 // 0008 - 0108
        ai::sight::C_HumanDetector *m_pHumanDetector;                     // 0108 - 0110
        game::ai::sight::C_ActorEmitter *m_pUnkActorEmitter1;             // 0110 - 0118
        char pad1[0x8];                                                   // 0118 - 0120
        game::ai::hear::C_ActorEar *m_pActorEar;                          // 0120 - 0128
        C_HumanInventory *m_pHumanInventory;                              // 0128 - 0130
        C_HumanScript *m_pHumanScript;                                    // 0130 - 0138
        C_HumanAIController *m_pHumanAIController;                        // 0138 - 0140
        char pad2[0x8];                                                   // 0140 - 0148
        C_HumanWeaponController *m_pHumanWeaponController;                // 0148 - 0150
        ue::game::injury::C_InjuryAnalyser *m_pInjuryAnalyser;            // 0150 - 0158
        C_HumanHeadController *m_pHumanHeadController;                    // 0158 - 0160
        void *m_pUnkPtr;                                                  // 0160 - 0168
        C_CorpseEmitter *m_pCorpseEmitter;                                // 0168 - 0170
        void *m_pUnkActorEmitter2;                                        // 0170 - 0178
        void *m_pUnkActorEmitter3;                                        // 0178 - 0180
        void *m_pUnkActorEmitter4;                                        // 0180 - 0188
        game::ai::sight::C_ActorEmitter *m_pBattleSearchEmitter;          // 0188 - 0190
        void *m_pUnkActorEmitter5;                                        // 0190 - 0198
        void *m_pUnkActorEmitter6;                                        // 0198 - 01A0
        void *m_pUnkActorEmitter7;                                        // 01A0 - 01A8
        C_WeaponEmitter *m_pWeaponEmitter;                                // 01A8 - 01B0
        char pad3[0x8];                                                   // 01B0 - 01B8
        C_MeleeEmitter *m_pMeleeEmitter;                                  // 01B8 - 01C0
        char pad4[0x8];                                                   // 01C0 - 01C8
        ue::game::humanai::C_CharacterState *m_pCharacterState;           // 01C8 - 01D0
        C_EntityOwnershipHumanCar *m_pEntityOwnershipHumanCar;            // 01D0 - 01D8
        ue::game::human::C_BehaviorCharacter *m_pBehaviorCharacter;       // 01D8 - 01E0
        float m_fHealth;                                                  // 01E0 - 01E4
        float m_fHealthMax;                                               // 01E4 - 01E8
        char pad6[0x10];                                                  // 01E8 - 01F8
        bool m_bInvulnerable;                                             // 01F8 - 01F9
        bool m_bDeath;                                                    // 01F9 - 01FA
        char pad7[0x1];                                                   // 01FA - 01FB
        bool m_bDemigod;                                                  // 01FB - 01FC
        char pad8[0xA4];                                                  // 01FC - 02A0
        mafia::health_system::C_HealthSystem *m_pHealthSystem;            // 02A0 - 02A8
        char pad82[0x10];                                                 // 02A8 - 02B8
        float field_02B8;                                                 // 02B8 - 02BC
        float m_fWaterLevel;                                              // 02BC - 02C0
        bool m_bWasCarried;                                               // 02E2 - 02E3
        char pad9[0x65];                                                  // 02E3 - 0348
        ue::game::human::C_Agent *m_pAgent;                               // 0348 - 0350
        char pad10[0x34];                                                 // 0350 - 0384
        float m_fTransparency;                                            // 0384 - 0388
        char pad11[0x10];                                                 // 0388 - 0398
        C_Entity *m_pLastGroundEntity;                                    // 0398 - 03A0
        C_Entity *m_pInteractedEntity;                                    // 03A0 - 03A8
        char pad12[0xD0];                                                 // 03A8 - 0478
        ue::game::humanai::C_CharacterController *m_pCharacterController; // 0478 - 0480
        char pad13[0x18];                                                 // 0480 - 0498
        ue::game::audio::radio::C_RadioSound *m_pRadioSound;              // 0498 - 04A0
        char pad14[0x8];                                                  // 04A0 - 04A8
        ue::sys::core::C_ModelComponent *m_pModelComponent;               // 04A8 - 04B0

      public:
        virtual void TickPrePhysics(float, /*ue::sys::math::C_Frustum const**/ void *, /*ue::sys::math::C_Frustum const**/ void *)           = 0;
        virtual void TickPostPhysics(float)                                                                                                  = 0;
        virtual void TickPostPhysicsCamera(float)                                                                                            = 0;
        virtual void *GetHumanActivityInspect()                                                                                              = 0;
        virtual void *__UNK_VIRTUAL_FN_480(void *)                                                                                           = 0;
        virtual void *GetAgent()                                                                                                             = 0;
        virtual bool __UNK_VIRTUAL_FN_496()                                                                                                  = 0;
        virtual bool __UNK_VIRTUAL_FN_504()                                                                                                  = 0;
        virtual bool __UNK_VIRTUAL_FN_512()                                                                                                  = 0;
        virtual bool __UNK_VIRTUAL_FN_520()                                                                                                  = 0;
        virtual void *GetGroundMaterialInfo()                                                                                                = 0;
        virtual void *GetValidGroundMaterialInfo()                                                                                           = 0;
        virtual C_Entity *GetLastGroundEntity()                                                                                              = 0;
        virtual bool HasTagWithValue(/*ue::sys::utils::C_HashNameString const&*/ void *, /*ue::sys::utils::C_HashNameString const&*/ void *) = 0;
        virtual void *GetTagValue(void *, void *)                                                                                            = 0;
        virtual void RemoveTag(void *, void *)                                                                                               = 0;
        virtual void AddTag(/*ue::sys::utils::C_HashNameString const&*/ void *, /*ue::sys::utils::C_HashNameString const&*/ void *) = 0;
        virtual bool CanCarryBody()                                                                                                          = 0;
        virtual bool CanDropBody()                                                                                                           = 0;
        virtual bool IsCarryingBody()                                                                                                        = 0;
        virtual unsigned char __UNK_VIRTUAL_FN_608(I_Human2 *, bool)                                                                         = 0;
        virtual void StartCarryBody(I_Human2 *, bool)                                                                                        = 0;
        virtual void FinishStartCarryBody(I_Human2 *, bool)                                                                                  = 0;
        virtual void StopCarryBody(bool)                                                                                                     = 0;
        virtual void FinishStopCarryBody()                                                                                                   = 0;
        virtual I_Human2 *GetCarriedBody()                                                                                                   = 0;
        virtual void PickupBodyInterrupted()                                                                                                 = 0;
        virtual void SetWasCarried()                                                                                                         = 0;
        virtual void *GetHealthSystem()                                                                                                      = 0;
        virtual void SetMaxAnimLod(bool)                                                                                                     = 0;
        virtual bool GetMaxAnimLod()                                                                                                         = 0;
        virtual void SetMaxMeshLod(bool)                                                                                                     = 0;
        virtual bool GetMaxMeshLod()                                                                                                         = 0;
        virtual ue::game::humanai::C_CharacterController *GetCharacterController()                                                           = 0;
        virtual ue::sys::math::C_Vector GetBoneWorldPos(uint64_t)                                                                            = 0;
        virtual void GetBoneWorldMatrix(uint64_t, ue::sys::math::C_Matrix &)                                                                 = 0;
        virtual void GetFutureBoneModelMatrix(uint64_t, uint64_t, float, float, ue::sys::math::C_Matrix &)                                   = 0;
        virtual void GetFutureBoneModelMatrix(uint64_t, uint64_t, float, ue::sys::math::C_Matrix &)                                          = 0;
        virtual void GetDeltaMatrix(uint64_t, float, float, ue::sys::math::C_Matrix &)                                                       = 0;
        virtual /*ue::game::physics::E_PriorityGroup*/ unsigned char GetCollisionPriorityGroup()                                             = 0;
        virtual void SetCollisionPriorityGroup(/*ue::game::physics::E_PriorityGroup*/ unsigned char) = 0;
        virtual ue::sys::utils::C_HashName GetBoneHashFromEnum(/*I_Human2::E_PosRequestType*/ int) = 0;
        virtual ue::sys::math::C_Vector GetPosRequest(/*I_Human2::E_PosRequestType*/ int) = 0;
        virtual ue::sys::math::C_Vector GetDirRequest(/*I_Human2::E_DirRequestType*/ int) = 0;
        virtual ue::sys::math::C_Vector GetBoneWorldPosFromEnum(/*I_Human2::E_PosRequestType*/ int) = 0;
        virtual ue::sys::math::C_Vector *GetVelocity()                                                                                       = 0;
        virtual ue::sys::math::C_Vector GetOwnerVelocity()                                                                                   = 0;
        virtual bool IsInCar()                                                                                                               = 0;
        virtual bool IsInBoat()                                                                                                              = 0;
        virtual bool IsInVehicle()                                                                                                           = 0;
        virtual bool IsInCover()                                                                                                             = 0;
        /**
         * Unlike IsInCover, IsInCoverFull will remain true if the player is aiming
         *
         * @param bool If false, it waits until the animation is finished before changing the value
         */
        virtual bool IsInCoverFull(bool)                                                   = 0;
        virtual bool IsInCutscene()                                                        = 0;
        virtual void **GetRigidBody(void *)                                                = 0;
        virtual void Spawn()                                                               = 0;
        virtual ue::sys::math::C_Vector *GetGroundNormal()                                 = 0;
        virtual void SetTotalCollisionHeight(float)                                        = 0;
        virtual float GetTotalCollisionHeight()                                            = 0;
        virtual void UseMedkit()                                                           = 0;
        virtual void ThrowWeapon()                                                         = 0;
        virtual void SetTransparency(float)                                                = 0;
        virtual float GetTransparency()                                                    = 0;
        virtual void SetTransparencyTarget(float)                                          = 0;
        virtual void SetMirrorVisibility(bool)                                             = 0;
        virtual void SetDirtBlend(float)                                                   = 0;
        virtual bool IsUnderScriptControl()                                                = 0;
        virtual bool DoDamage(/*C_EntityMessageDamage**/ void *, bool) = 0;
        virtual bool IsStunAllowed()                                                       = 0;
        virtual bool AreControlsLocked()                                                   = 0;
        virtual bool IsProcessedByRender()                                                 = 0;
        virtual int64_t TimeFromProcessedByRender()                                        = 0;
        virtual bool PhysIsFalling()                                                       = 0;
        virtual bool IsSteppingOnCurb()                                                    = 0;
        virtual unsigned char __UNK_VIRTUAL_FN_1032()                                      = 0;
        virtual unsigned char __UNK_VIRTUAL_FN_1040()                                      = 0;
        virtual unsigned char __UNK_VIRTUAL_FN_1048()                                      = 0;
        virtual void DespawnHuman()                                                        = 0;
        virtual bool IsClothSimulated()                                                    = 0;
        virtual void SimulateCloth(bool, bool)                                             = 0;
        virtual void SetupHumanEnemyEmitters()                                             = 0;
        virtual bool LinkObject(uint64_t const &, ue::C_Ptr<ue::sys::core::C_SceneObject>) = 0;
        virtual bool UnlinkObject(ue::C_Ptr<ue::sys::core::C_SceneObject>)                 = 0;
        virtual void ActivateBattleSearch()                                                = 0;
        virtual void DeactivateBattleSearch()                                              = 0; // last

      public:
        C_HumanInventory *GetHumanInventory() {
            return m_pHumanInventory;
        }

        C_InventoryWrapper *GetInventoryWrapper() {
            return reinterpret_cast<C_InventoryWrapper *>(m_pHumanInventory);
        }

        C_HumanWeaponController *GetHumanWeaponController() {
            return m_pHumanWeaponController;
        }

        C_HumanScript *GetHumanScript() {
            return m_pHumanScript;
        }

        void *GetBehaviorCharacter() {
            return m_pBehaviorCharacter;
        }

        float GetDepthInWater() {
            return (m_fWaterLevel < 0.0f ? field_02B8 - *(float *)(*(uintptr_t *)m_pAgent->m_pAgentController + 8) : 0.0f);
        }
    };

    class C_Human2: public I_Human2 {
      public:
        virtual void LockControls(bool)                                 = 0;
        virtual bool IsControlLockFinished()                            = 0;
        virtual bool SetPlayerCtrlStyle(char const *)                   = 0;
        virtual char const *GetPlayerCtrlStyle()                        = 0;
        virtual void OnContactNormalModifiedToBlock()                   = 0;
        virtual ue::sys::math::C_Vector GetPosExtrapolated(float)       = 0;
        virtual void *GetGuiController()                                = 0;
        virtual bool IsCrouch()                                         = 0;
        virtual void OnTeleported()                                     = 0;
        virtual int GetDefaultCollisionPriorityGroup()                  = 0;
        virtual void SetPosPostPhys(ue::sys::math::C_Vector const &)    = 0;
        virtual bool ProcessNotification(short, unsigned short const &) = 0;
        virtual void SetInCutscene(bool)                                = 0;
        virtual bool ProcessNotificationCamera(short)                   = 0;
        virtual bool IsFollowingPath()                                  = 0;

        void EnableShadows(bool);
        void EnableHumanClothes();
    };
} // namespace SDK
