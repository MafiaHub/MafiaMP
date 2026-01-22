#pragma once

#include <cstdint>
#include "../../sys/math/c_vector.h"

namespace SDK {
    namespace ue::game::camera {
        /**
         * S_ForcedLook - Forced look state structure
         * Size: 0x60 (96 bytes)
         *
         * Reversed from ue::game::camera::S_ForcedLook::Reset (0x1409b2d50)
         * Size confirmed: C_Camera offset 0x298, next field at 0x2F8
         *
         * Field offsets from Reset function:
         * - 0x00: C_Vector (ZeroVector init)
         * - 0x0C: C_Vector (ZeroVector init)
         * - 0x18: QWORD = 0
         * - 0x20: QWORD (qword_14622E880)
         * - 0x28: QWORD (qword_14622E880)
         * - 0x30: QWORD (pointer, gets released via sub_140651860)
         * - 0x38: QWORD = 0
         * - 0x40: WORD = 0
         * - 0x48: DWORD = 1.0f
         * - 0x4C: DWORD = 1.0f
         * - 0x50: DWORD = 1.0f (QWORD write, high 4 bytes zeroed)
         * - 0x58: QWORD = 0
         */
        struct S_ForcedLook {
            ue::sys::math::C_Vector m_vDirection;                               // 0x00 - 0x0C
            ue::sys::math::C_Vector m_vTargetDirection;                         // 0x0C - 0x18
            uint64_t m_qwUnk18;                                                 // 0x18 - 0x20
            uint64_t m_qwUnk20;                                                 // 0x20 - 0x28
            uint64_t m_qwUnk28;                                                 // 0x28 - 0x30
            void *m_pRefCounted;                                                // 0x30 - 0x38
            uint64_t m_qwUnk38;                                                 // 0x38 - 0x40
            uint16_t m_wFlags;                                                  // 0x40 - 0x42
            uint16_t m_wPad42;                                                  // 0x42 - 0x44
            uint32_t m_dwPad44;                                                 // 0x44 - 0x48
            float m_fBlendValue1;                                               // 0x48 - 0x4C (initialized to 1.0f)
            float m_fBlendValue2;                                               // 0x4C - 0x50 (initialized to 1.0f)
            float m_fBlendValue3;                                               // 0x50 - 0x54 (initialized to 1.0f)
            uint32_t m_dwPad54;                                                 // 0x54 - 0x58
            uint64_t m_qwUnk58;                                                 // 0x58 - 0x60
        };

        static_assert(sizeof(S_ForcedLook) == 0x60, "S_ForcedLook size mismatch");
    } // namespace ue::game::camera
} // namespace SDK
