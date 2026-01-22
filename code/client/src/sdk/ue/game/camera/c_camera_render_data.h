#pragma once

#include <cstdint>

#include "../../sys/math/c_matrix.h"

namespace SDK {
    namespace ue::game::camera {
        /**
         * C_CameraRenderData - Internal render data structure
         * Located at C_Camera offset 0x90 (144)
         * Size: 0x100 (256 bytes)
         */
        struct C_CameraRenderData {
            void *m_pVtable;                                                // 0000 - 0008
            bool m_bFlag;                                                   // 0008 - 0009
            char pad0[0x57];                                                // 0009 - 0060
            uint64_t m_iUnk60;                                              // 0060 - 0068
            uint64_t m_iUnk68;                                              // 0068 - 0070
            float m_fUnk70;                                                 // 0070 - 0074
            float m_fUnk74;                                                 // 0074 - 0078
            uint64_t m_iUnk78;                                              // 0078 - 0080
            uint32_t m_iUnk80;                                              // 0080 - 0084
            uint16_t m_iUnk84;                                              // 0084 - 0086
            uint8_t m_bUnk86;                                               // 0086 - 0087
            char pad1[0x01];                                                // 0087 - 0088
            uint64_t m_iUnk88;                                              // 0088 - 0090
            sys::math::C_Matrix m_mViewMatrix;                              // 0090 - 00C0
            sys::math::C_Matrix m_mProjectionMatrix;                        // 00C0 - 00F0
            float m_fNearClip;                                              // 00F0 - 00F4
            bool m_bUnkF4;                                                  // 00F4 - 00F5
            char pad2[0x03];                                                // 00F5 - 00F8
            float m_fFOV;                                                   // 00F8 - 00FC
            char pad3[0x04];                                                // 00FC - 0100
        };

        static_assert(sizeof(C_CameraRenderData) == 0x100, "C_CameraRenderData size mismatch");
    } // namespace ue::game::camera
} // namespace SDK
