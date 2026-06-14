#pragma once

#include <cstdint>
#include "../../sys/math/c_matrix.h"

namespace SDK {
    namespace ue::game::camera {
        /**
         * C_CameraRenderData - Internal render data structure
         * Located at C_Camera offset 0x90 (144)
         * Size: 0x100 (256 bytes)
         *
         * Reversed from sub_140996330 (constructor)
         * Size confirmed: C_Camera offset 0x90, next field at 0x190
         *
         * Field offsets from constructor:
         * - 0x00: vtable
         * - 0x08: bool
         * - 0x60: QWORD (zeroed)
         * - 0x68: QWORD (zeroed)
         * - 0x70: float = 1.0f
         * - 0x74: float = 1.0f
         * - 0x78: float = 0.0f (from QWORD write at 0x74)
         * - 0x80: DWORD = 0
         * - 0x84: WORD = 0
         * - 0x86: BYTE = 0
         * - 0x88: QWORD = 0
         * - 0x90: C_Matrix (48 bytes) - Identity
         * - 0xC0: C_Matrix (48 bytes) - Identity
         * - 0xF0: float = 70.0f (FOV)
         * - 0xF4: bool
         * - 0xF8: float = 50.0f
         */
        struct C_CameraRenderData {
            void *m_pVtable;                                                    // 0x00 - 0x08
            bool m_bUnk08;                                                      // 0x08 - 0x09
            uint8_t m_pad09[0x57];                                              // 0x09 - 0x60
            uint64_t m_qwUnk60;                                                 // 0x60 - 0x68
            uint64_t m_qwUnk68;                                                 // 0x68 - 0x70
            float m_fScaleX;                                                    // 0x70 - 0x74 (init 1.0f)
            float m_fScaleY;                                                    // 0x74 - 0x78 (init 1.0f)
            float m_fScaleZ;                                                    // 0x78 - 0x7C (init 0.0f)
            uint32_t m_dwUnk7C;                                                 // 0x7C - 0x80
            uint32_t m_dwUnk80;                                                 // 0x80 - 0x84
            uint16_t m_wUnk84;                                                  // 0x84 - 0x86
            uint8_t m_bUnk86;                                                   // 0x86 - 0x87
            uint8_t m_pad87;                                                    // 0x87 - 0x88
            uint64_t m_qwUnk88;                                                 // 0x88 - 0x90
            ue::sys::math::C_Matrix m_mViewMatrix;                              // 0x90 - 0xC0
            ue::sys::math::C_Matrix m_mProjectionMatrix;                        // 0xC0 - 0xF0
            float m_fFOV;                                                       // 0xF0 - 0xF4 (init 70.0f)
            bool m_bUnkF4;                                                      // 0xF4 - 0xF5
            uint8_t m_padF5[0x03];                                              // 0xF5 - 0xF8
            float m_fNearClip;                                                  // 0xF8 - 0xFC (init 50.0f)
            uint32_t m_padFC;                                                   // 0xFC - 0x100
        };

        static_assert(sizeof(C_CameraRenderData) == 0x100, "C_CameraRenderData size mismatch");
    } // namespace ue::game::camera
} // namespace SDK
