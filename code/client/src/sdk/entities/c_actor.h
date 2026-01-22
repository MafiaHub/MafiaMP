#pragma once

#include <sdk/ue/sys/math/c_matrix.h>
#include <sdk/ue/sys/math/c_quat.h>
#include <sdk/ue/sys/core/c_frame.h>
#include "c_entity_pos.h"

#include <cstdint>

namespace SDK {
    /**
     * C_Actor - Base class for actors (entities with position and transform)
     *
     * Size: 0xF8 (248 bytes)
     *
     * Members:
     *   0xE0: I_ActorActions* - Interface vtable for actor actions (type name: E_ActorActions)
     *   0xE8: S_ActorStreamingInfo* - Streaming info structure (owner at +0x18, activated flag at +0x168)
     *   0xF0: C_Actor* - Direct owner pointer (used by I_Human2::SetOwner)
     */
    class C_Actor: public C_EntityPos {
      public:
        virtual void SetPos(ue::sys::math::C_Vector const &)                                                                              = 0;
        virtual void SetDir(ue::sys::math::C_Vector const &)                                                                              = 0;
        virtual void SetRot(ue::sys::math::C_Quat const &)                                                                                = 0;
        virtual void SetScale(float)                                                                                                      = 0;
        virtual void SetTransform(ue::sys::math::C_Matrix const &)                                                                        = 0;
        virtual ue::sys::math::C_Vector GetPos(void)                                                                                      = 0;
        virtual ue::sys::math::C_Vector GetDir(void)                                                                                      = 0;
        virtual ue::sys::math::C_Quat GetRot(void)                                                                                        = 0;
        virtual ue::sys::math::C_Vector GetScale(void)                                                                                    = 0;
        virtual void GetBodyPartPos(unsigned int, ue::sys::math::C_Vector &)                                                              = 0;
        virtual ue::sys::core::C_Frame *GetFrame(void)                                                                                    = 0;
        virtual void GameLoadPRS(/*ue::sys::utils::C_BitStream**/ void *) = 0;
        virtual ue::sys::math::C_Vector GetCameraPoint(void)                                                                              = 0;
        virtual void __UNK_VIRTUAL_FN_352()                                                                                               = 0;
        virtual void SetOwner(C_Entity *)                                                                                                 = 0;
        virtual void SetPosAndDir(ue::sys::math::C_Vector const &, ue::sys::math::C_Vector const &, ue::sys::core::E_TransformChangeType) = 0;
        virtual void *GetSubDivObj(void)                                                                                                  = 0;
        virtual bool IsDeath(void)                                                                                                        = 0;
        virtual void __UNK_VIRTUAL_FN_392()                                                                                               = 0;
        virtual bool IsPausedPhysics(void)                                                                                                = 0;
        virtual void TryPausePhysics(bool)                                                                                                = 0;
        virtual bool AreActionsDisabled(void)                                                                                             = 0;
        virtual void EnableUnderwaterDetection(bool)                                                                                      = 0;
        virtual void *GetUnderwaterStatus(void)                                                                                           = 0;
        virtual bool __UNK_VIRTUAL_FN_440(uint8_t, uint8_t, void *, void *)                                                               = 0;

        C_Actor *GetOwner() {
            return m_pActorStreamingInfo != nullptr ? *(C_Actor **)((uint64_t)m_pActorStreamingInfo + 0x18) : nullptr;
        }

        C_Actor *GetDirectOwner() {
            return m_pOwner;
        }

      protected:
        void *m_pActorActions;                               // 0xE0 - 0xE8 (I_ActorActions interface vtable)
        void *m_pActorStreamingInfo;                         // 0xE8 - 0xF0 (S_ActorStreamingInfo* - has owner at +0x18, activated flag at +0x168)
        C_Actor *m_pOwner;                                   // 0xF0 - 0xF8 (Direct owner actor pointer)
    };

    static_assert(sizeof(C_Actor) == 0xF8, "C_Actor size mismatch");
} // namespace SDK
