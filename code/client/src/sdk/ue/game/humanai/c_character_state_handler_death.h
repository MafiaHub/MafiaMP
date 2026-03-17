#pragma once

#include "c_character_state_handler.h"

namespace SDK {
    namespace ue::game::humanai {
        // Size: ~216 bytes (0xD8)
        class C_CharacterStateHandlerDeath : public C_CharacterStateHandler {
          public:
            enum E_State {
                E_DS_NONE    = 0,
                E_DS_AGONY   = 1,
                E_DS_RAGDOLL = 2,
                E_DS_DEAD    = 3
            };

          protected:
            uint16_t m_unk64;                 // 0x40 - 0x42
            uint8_t m_unk66;                  // 0x42 - 0x43
            char _pad0[0x5];                  // 0x43 - 0x48
            bool m_bResetOnActivate;          // 0x48 - 0x49 (default true)
            char _pad1[0x3];                  // 0x49 - 0x4C
            uintptr_t m_unk76;                // 0x4C - 0x54
            uint32_t m_unk84;                 // 0x54 - 0x58
            float m_fAgonyTimer;              // 0x58 - 0x5C (default +INF)
            float m_fDeathTimer;              // 0x5C - 0x60 (default +INF)
            uint32_t m_unk96;                 // 0x60 - 0x64
            uintptr_t m_unk100;               // 0x64 - 0x6C
            int32_t m_nDeathType;             // 0x6C - 0x70 (default -1)
            int32_t m_nStateType;             // 0x70 - 0x74 (default 1)
            bool m_bUnk116;                   // 0x74 - 0x75
            uint16_t m_unk117;                // 0x75 - 0x77
            bool m_bStunned;                  // 0x77 - 0x78
            uint16_t m_unk120;                // 0x78 - 0x7A
            char _pad2[0x6];                  // 0x7A - 0x80
            void *m_pSyncObject;              // 0x80 - 0x88
            void *m_pDeathSyncObject;         // 0x88 - 0x90
            char _pad3[0x24];                 // 0x90 - 0xB4
            uintptr_t m_unk180;               // 0xB4 - 0xBC
            uint32_t m_unk188;                // 0xBC - 0xC0
            int64_t m_unk192;                 // 0xC0 - 0xC8 (default -1)
            uintptr_t m_unk200;               // 0xC8 - 0xD0
            bool m_bUnk208;                   // 0xD0 - 0xD1
            char _pad4[0x7];                  // 0xD1 - 0xD8

          public:
            void Reset();
            void DropWeapon();
            void MeleeDeath();
            void RegularDeath();
            void PassiveRagdollDeath();
            void PlayDeathRattle();
            float GetAgonyDuration();
            void OnStun();
        };
    } // namespace ue::game::humanai
} // namespace SDK
