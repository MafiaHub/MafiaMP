#pragma once

#include "c_character_state_handler.h"
#include "../../sys/math/c_vector.h"

namespace SDK {
    namespace ue::game::humanai {
        // Size: ~256 bytes (0x100)
        class C_CharacterStateHandlerPlayerReaction : public C_CharacterStateHandler {
          protected:
            uint16_t m_unk64;                     // 0x40 - 0x42
            uint8_t m_unk66;                      // 0x42 - 0x43
            char _pad0[0x5];                      // 0x43 - 0x48
            uintptr_t m_unk72;                    // 0x48 - 0x50
            bool m_bReactionActive;               // 0x50 - 0x51
            char _pad1[0x7];                      // 0x51 - 0x58
            void *m_pReactionSource1;             // 0x58 - 0x60
            float m_fSource1Distance;             // 0x60 - 0x64
            bool m_bSource1Active;                // 0x64 - 0x65
            char _pad2[0x3];                      // 0x65 - 0x68
            ue::sys::math::C_Vector m_Source1Pos; // 0x68 - 0x74
            char _pad3[0x4];                      // 0x74 - 0x78
            void *m_pReactionSource2;             // 0x78 - 0x80
            float m_fSource2Distance;             // 0x80 - 0x84
            bool m_bSource2Active;                // 0x84 - 0x85
            char _pad4[0x3];                      // 0x85 - 0x88
            uintptr_t m_unk136;                   // 0x88 - 0x90
            ue::sys::math::C_Vector m_Source2Pos; // 0x90 - 0x9C
            ue::sys::math::C_Vector m_ReactionDir; // 0x9C - 0xA8
            void *m_pReactionSource3;             // 0xA8 - 0xB0
            float m_fSource3Distance;             // 0xB0 - 0xB4
            bool m_bSource3Active;                // 0xB4 - 0xB5
            char _pad5[0x3];                      // 0xB5 - 0xB8
            uint32_t m_nReactionFlags;            // 0xB8 - 0xBC
            char _pad6[0x4];                      // 0xBC - 0xC0
            uintptr_t m_unk192;                   // 0xC0 - 0xC8
            int32_t m_nReactionState;             // 0xC8 - 0xCC (default -1)
            uint32_t m_unk204;                    // 0xCC - 0xD0
            bool m_bReactionUnk;                  // 0xD0 - 0xD1
            char _pad7[0x3];                      // 0xD1 - 0xD4
            ue::sys::math::C_Vector m_TargetPos;  // 0xD4 - 0xE0
            uintptr_t m_unk224;                   // 0xE0 - 0xE8
            uintptr_t m_unk232;                   // 0xE8 - 0xF0
            uintptr_t m_unk240;                   // 0xF0 - 0xF8
            uintptr_t m_unk248;                   // 0xF8 - 0x100
        };
    } // namespace ue::game::humanai
} // namespace SDK
