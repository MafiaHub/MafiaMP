#pragma once

#include "c_character_state_handler.h"

namespace SDK {
    namespace ue::game::humanai {
        // Size: ~360 bytes (0x168)
        class C_CharacterStateHandlerGrenade : public C_CharacterStateHandler {
          protected:
            uint16_t m_unk64;               // 0x40 - 0x42
            uint8_t m_unk66;                // 0x42 - 0x43
            char _pad0[0x5];                // 0x43 - 0x48
            uintptr_t m_unk72;              // 0x48 - 0x50
            uintptr_t m_unk80;              // 0x50 - 0x58
            uintptr_t m_unk88;              // 0x58 - 0x60
            uintptr_t m_unk96;              // 0x60 - 0x68
            uintptr_t m_unk104;             // 0x68 - 0x70
            uintptr_t m_unk112;             // 0x70 - 0x78
            uintptr_t m_unk120;             // 0x78 - 0x80
            uintptr_t m_unk128;             // 0x80 - 0x88
            uintptr_t m_unk136;             // 0x88 - 0x90
            uintptr_t m_unk144;             // 0x90 - 0x98
            uintptr_t m_unk152;             // 0x98 - 0xA0
            uintptr_t m_unk160;             // 0xA0 - 0xA8
            bool m_bGrenadeActive;          // 0xA8 - 0xA9
            char _pad1[0x3];                // 0xA9 - 0xAC
            uintptr_t m_unk172;             // 0xAC - 0xB4
            uintptr_t m_unk180;             // 0xB4 - 0xBC
            uintptr_t m_unk188;             // 0xBC - 0xC4
            uintptr_t m_unk196;             // 0xC4 - 0xCC
            uintptr_t m_unk204;             // 0xCC - 0xD4
            uintptr_t m_unk212;             // 0xD4 - 0xDC
            bool m_bThrowReady;             // 0xDC - 0xDD
            char _pad2[0x3];                // 0xDD - 0xE0
            uint32_t m_nGrenadeState;       // 0xE0 - 0xE4
            int32_t m_nGrenadeType;         // 0xE4 - 0xE8 (default -1)
            uintptr_t m_unk232;             // 0xE8 - 0xF0
            uintptr_t m_unk240;             // 0xF0 - 0xF8
            uintptr_t m_unk248;             // 0xF8 - 0x100
            uintptr_t m_unk256;             // 0x100 - 0x108
            uint32_t m_unk264;              // 0x108 - 0x10C
            bool m_bUnk268;                 // 0x10C - 0x10D
            char _pad3[0x3];                // 0x10D - 0x110
            uint32_t m_unk272;              // 0x110 - 0x114
            bool m_bUnk276;                 // 0x114 - 0x115
            char _pad4[0x3];                // 0x115 - 0x118
            uintptr_t m_unk280;             // 0x118 - 0x120
            uint16_t m_unk288;              // 0x120 - 0x122
            bool m_bGrenadeUnk;             // 0x122 - 0x123
            char _pad5[0x1];                // 0x123 - 0x124
            uint32_t m_unk292;              // 0x124 - 0x128
            float m_fThrowForce;            // 0x128 - 0x12C (default -1.0f)
            float m_fThrowAngle;            // 0x12C - 0x130 (default -1.0f)
            float m_fThrowRange;            // 0x130 - 0x134
            int32_t m_nThrowState;          // 0x134 - 0x138 (default -1)
            uintptr_t m_unk312;             // 0x138 - 0x140
            uintptr_t m_unk320;             // 0x140 - 0x148
            void *m_pGrenadeTrajectory;     // 0x148 - 0x150
            uintptr_t m_unk336;             // 0x150 - 0x158
            uintptr_t m_unk344;             // 0x158 - 0x160
            uintptr_t m_unk352;             // 0x160 - 0x168

          public:
            bool IsThrowingGrenade() const;
        };
    } // namespace ue::game::humanai
} // namespace SDK
