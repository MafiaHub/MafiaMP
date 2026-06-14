#pragma once

#include "c_character_state_handler.h"

namespace SDK {
    namespace ue::game::humanai {
        // Size: 104 bytes (0x68)
        class C_CharacterStateHandlerDowned : public C_CharacterStateHandler {
          protected:
            uint16_t m_unk64;            // 0x40 - 0x42
            uint8_t m_unk66;             // 0x42 - 0x43
            char _pad0[0x5];             // 0x43 - 0x48
            uintptr_t m_unk72;           // 0x48 - 0x50
            uintptr_t m_unk80;           // 0x50 - 0x58
            int32_t m_nDownedState;      // 0x58 - 0x5C (default -1)
            uint32_t m_unk92;            // 0x5C - 0x60
            char _pad1[0x4];             // 0x60 - 0x64
            uint16_t m_unk100;           // 0x64 - 0x66
            char _pad2[0x2];             // 0x66 - 0x68
        };
    } // namespace ue::game::humanai
} // namespace SDK
