#pragma once

#include "c_character_state_handler.h"

namespace SDK {
    namespace ue::game::humanai {
        // Size: ~232 bytes (0xE8)
        class C_CharacterStateHandlerLookAt : public C_CharacterStateHandler {
          protected:
            uint16_t m_unk64;             // 0x40 - 0x42
            uint8_t m_unk66;              // 0x42 - 0x43
            char _pad0[0x5];              // 0x43 - 0x48
            uintptr_t m_unk72;            // 0x48 - 0x50
            uint32_t m_nLookAtMode;       // 0x50 - 0x54
            char _pad1[0x4];              // 0x54 - 0x58
            uintptr_t m_unk88;            // 0x58 - 0x60
            uintptr_t m_unk96;            // 0x60 - 0x68
            float m_fYawSpeed;            // 0x68 - 0x6C (default 1.0f)
            uint32_t m_unk108;            // 0x6C - 0x70
            uint32_t m_unk112;            // 0x70 - 0x74
            float m_fPitchSpeed;          // 0x74 - 0x78 (default 1.0f)
            uint32_t m_unk120;            // 0x78 - 0x7C
            uint32_t m_unk124;            // 0x7C - 0x80
            int32_t m_nLookAtState;       // 0x80 - 0x84 (default 1)
            float m_fMaxAngle;            // 0x84 - 0x88 (default 2*PI)
            uint16_t m_unk136;            // 0x88 - 0x8A
            char _pad2[0x2];              // 0x8A - 0x8C
            uintptr_t m_unk140;           // 0x8C - 0x94
            uint32_t m_unk148;            // 0x94 - 0x98
            char _pad3[0x18];             // 0x98 - 0xB0
            bool m_bLookAtActive;         // 0xB0 - 0xB1
            char _pad4[0x3];              // 0xB1 - 0xB4
            uintptr_t m_unk180;           // 0xB4 - 0xBC
            char _pad5[0x4];              // 0xBC - 0xC0
            uintptr_t m_unk192;           // 0xC0 - 0xC8
            uintptr_t m_unk200;           // 0xC8 - 0xD0
            uint16_t m_unk208;            // 0xD0 - 0xD2
            char _pad6[0x6];              // 0xD2 - 0xD8
            uintptr_t m_unk216;           // 0xD8 - 0xE0
            uintptr_t m_unk224;           // 0xE0 - 0xE8

          public:
            void OnEnableEyeLookAt();
            void OnDisableEyeLookAt();
        };
    } // namespace ue::game::humanai
} // namespace SDK
