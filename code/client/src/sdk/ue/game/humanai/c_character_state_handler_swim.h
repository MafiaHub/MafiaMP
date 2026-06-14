#pragma once

#include "c_character_state_handler_base_locomotion.h"

namespace SDK {
    namespace ue::game::humanai {
        // Size: ~464 bytes (0x1D0)
        class C_CharacterStateHandlerSwim : public C_CharacterStateHandlerBaseLocomotion {
          protected:
            char m_SwimData[0x40];        // 0x100 - 0x140
            uint16_t m_unk320;            // 0x140 - 0x142
            char _pad0[0x2];              // 0x142 - 0x144
            int32_t m_nSwimState;         // 0x144 - 0x148 (default -1)
            int32_t m_nSwimMode;          // 0x148 - 0x14C (default 1)
            char _pad1[0x58];             // 0x14C - 0x1A4
            uintptr_t m_unk420;           // 0x1A4 - 0x1AC
            uintptr_t m_unk428;           // 0x1AC - 0x1B4
            char _pad2[0x4];              // 0x1B4 - 0x1B8
            uintptr_t m_unk440;           // 0x1B8 - 0x1C0
            uint16_t m_unk448;            // 0x1C0 - 0x1C2
            bool m_bSwimUnk;              // 0x1C2 - 0x1C3
            char _pad3[0x5];              // 0x1C3 - 0x1C8
            uintptr_t m_unk456;           // 0x1C8 - 0x1D0
        };
    } // namespace ue::game::humanai
} // namespace SDK
