#pragma once

#include <cstdint>

#include "sdk/ue/sys/math/c_quat.h"
#include "sdk/ue/sys/math/c_vector.h"

namespace SDK {
    namespace ue::game::translocator {
        struct S_TranslocableObject {
            void *m_pSceneObject;           // 0x00 - 0x08
            void *m_pDescription;           // 0x08 - 0x10
            ue::sys::math::C_Vector m_vPos; // 0x10 - 0x1C
            float m_fUnk1C;                 // 0x1C - 0x20
            ue::sys::math::C_Quat m_qRot;   // 0x20 - 0x30
            float m_fScale;                 // 0x30 - 0x34
            uint32_t m_nFlags;              // 0x34 - 0x38
            void *m_pSDSResource;           // 0x38 - 0x40
            uint32_t m_nSDSIndex;           // 0x40 - 0x44
            char _pad44[0x04];              // 0x44 - 0x48
            void *m_pLODInfo;               // 0x48 - 0x50
            uint32_t m_nState;              // 0x50 - 0x54
            char _pad54[0x04];              // 0x54 - 0x58
        };

        static_assert(sizeof(S_TranslocableObject) == 0x58, "S_TranslocableObject size mismatch");

    } // namespace ue::game::translocator
} // namespace SDK
