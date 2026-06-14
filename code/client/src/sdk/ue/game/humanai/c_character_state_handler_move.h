#pragma once

#include "c_character_state_handler_base_locomotion.h"

namespace SDK {
    namespace ue::game::humanai {
        // Size: ~360 bytes (0x168)
        class C_CharacterStateHandlerMove : public C_CharacterStateHandlerBaseLocomotion {
          protected:
            uint32_t m_nMoveFlags;              // 0x100 - 0x104
            char _pad0[0x34];                   // 0x104 - 0x138
            uintptr_t m_unk312;                 // 0x138 - 0x140
            uintptr_t m_unk320;                 // 0x140 - 0x148
            uint16_t m_unk328;                  // 0x148 - 0x14A
            char _pad1[0xE];                    // 0x14A - 0x158
            bool m_bTurnActive;                 // 0x158 - 0x159
            char _pad2[0x3];                    // 0x159 - 0x15C
            uint32_t m_nTurnFlags;              // 0x15C - 0x160
            uint16_t m_unk352;                  // 0x160 - 0x162
            bool m_bSprintRequested;            // 0x162 - 0x163
            char _pad3[0x5];                    // 0x163 - 0x168

          public:
            bool IsSprinting() const;
            bool SharpTurnTransitionActive();
            bool SharpTurnDockingRangeActive();
            bool IsMoveTurnActive();
            bool IsMoveTurnDockingActive();
            bool CanCheckSlope();
            void ProceduralLeanDisabled();
        };
    } // namespace ue::game::humanai
} // namespace SDK
