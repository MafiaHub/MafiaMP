#pragma once

#include "c_character_state_handler_base_locomotion.h"
#include "../../c_second_order_filter.h"

namespace SDK {
    namespace ue::game::humanai {
        // Size: ~632 bytes (0x278)
        class C_CharacterStateHandlerTurret : public C_CharacterStateHandlerBaseLocomotion {
          protected:
            uintptr_t m_unk256;                        // 0x100 - 0x108
            uintptr_t m_unk264;                        // 0x108 - 0x110
            uint16_t m_unk272;                         // 0x110 - 0x112
            char _pad0[0x6];                           // 0x112 - 0x118
            uintptr_t m_unk280;                        // 0x118 - 0x120
            uintptr_t m_unk288;                        // 0x120 - 0x128
            uint16_t m_unk296;                         // 0x128 - 0x12A
            char _pad1[0x6];                           // 0x12A - 0x130
            uintptr_t m_unk304;                        // 0x130 - 0x138
            uintptr_t m_unk312;                        // 0x138 - 0x140
            uintptr_t m_unk320;                        // 0x140 - 0x148
            uintptr_t m_unk328;                        // 0x148 - 0x150
            uintptr_t m_unk336;                        // 0x150 - 0x158
            char m_TurretLimits[0x30];                 // 0x158 - 0x188 (pitch/yaw limit data)
            char _pad2[0x8];                           // 0x188 - 0x190
            ue::C_SecondOrderFilter m_YawFilter;       // 0x190 - 0x1F0
            ue::C_SecondOrderFilter m_PitchFilter;     // 0x1F0 - 0x250
            uintptr_t m_unk592;                        // 0x250 - 0x258
            uintptr_t m_unk600;                        // 0x258 - 0x260
            uintptr_t m_unk608;                        // 0x260 - 0x268
            uint32_t m_nTurretFlags;                   // 0x268 - 0x26C (default 0x10000)
            bool m_bTurretActive;                      // 0x26C - 0x26D
            char _pad3[0x3];                           // 0x26D - 0x270
            uintptr_t m_unk624;                        // 0x270 - 0x278
        };
    } // namespace ue::game::humanai
} // namespace SDK
