#pragma once

#include "../ue/sys/math/c_matrix.h"
#include "../ue/sys/math/c_quat.h"
#include "../ue/sys/core/c_frame.h"
#include "c_entity.h"

#include <cstdint>

namespace SDK {
    class C_Actor: public C_Entity {
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
            auto unk = *(uint64_t *)((uintptr_t)this + 0x238);
            return unk != 0x0 ? *(C_Actor **)((uint64_t)unk + 0x18) : nullptr;
        }
    };
} // namespace SDK
