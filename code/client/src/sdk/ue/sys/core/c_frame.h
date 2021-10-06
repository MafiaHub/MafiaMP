#pragma once

#include "../math/c_matrix.h"

namespace SDK {
    class C_Frame {
      public:
        virtual ~C_Frame()                                                                                                                     = default;
        virtual void *GetClassTypeInfo(void)                                                                                                   = 0;
        virtual void *GetTypeInfo(void)                                                                                                        = 0;
        virtual void *GetAABox(void)                                                                                                           = 0;
        virtual void SetWorldMatrix(ue::sys::math::C_Matrix const &)                                                                           = 0;
        virtual void *GetWorldAABox(void)                                                                                                      = 0;
        virtual void *GetWorldAABox(/*ue::sys::math::C_AABB&*/ void *, ue::sys::math::C_Matrix const &) = 0;
        virtual void *GetType(void)                                                                                                            = 0;
        virtual unsigned int GetFrameTypeID(void)                                                                                              = 0;
        virtual void *LinkTo(C_Frame *, unsigned int, bool)                                                                                    = 0;
        virtual void *SetLODGroup(/*ue::C_WeakPtr<ue::sys::core::C_LODGroup*/ void *weakPtrLod, /*ue::S_SingleThreadedPolicy, 1>*/ void *) = 0;
        virtual void *Collide(/*ue::spatial::I_MeshCDQueryBase**/ void *) = 0;
        virtual void *IsProcessedByRender(void)                                                                                                = 0;
        virtual void *OnAddedToScene(/*ue::sys::core::C_Scene**/) = 0;
        virtual void *OnRemoveFromScene(/*ue::sys::core::C_Scene**/) = 0;
        virtual void *OnLinkedToModel(/*ue::sys::core::C_Model**/) = 0;
        virtual void *OnUnlinkedFromModel(/*ue::sys::core::C_Model**/) = 0;
        virtual void *OnRelinkedFromModelToModel(/*ue::sys::core::C_Model*, ue::sys::core::C_Model**/) = 0;
        virtual void *OnLinkChanged(void)                                                                                                      = 0;
        virtual void *OnRenderModeChanged(void)                                                                                                = 0;
        virtual void *OnEnabledChanged(void)                                                                                                   = 0;
        virtual void *OnPRSChanged(void)                                                                                                       = 0;
        virtual void *OnVolumeChanged(void)                                                                                                    = 0;
        virtual void *DbgCheckDataConsistency(/*std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>&, uint*/) = 0;
        virtual void *DuplicateV(C_Frame const *)                                                                                              = 0;
    };
} // namespace SDK