#pragma once

#include "c_character_state_handler_base_locomotion.h"

namespace SDK {
    namespace ue::game::humanai {
        // Size: ~312 bytes (0x138)
        class C_CharacterStateHandlerPlayAnimation : public C_CharacterStateHandlerBaseLocomotion {
          protected:
            uintptr_t m_unk256;           // 0x100 - 0x108
            uintptr_t m_unk264;           // 0x108 - 0x110
            uintptr_t m_unk272;           // 0x110 - 0x118
            bool m_bStopped;              // 0x118 - 0x119
            char _pad0[0x7];              // 0x119 - 0x120
            uintptr_t m_pSyncObject;      // 0x120 - 0x128
            uintptr_t m_unk296;           // 0x128 - 0x130
            uint16_t m_unk304;            // 0x130 - 0x132
            bool m_bAnimUnk;              // 0x132 - 0x133
            char _pad1[0x5];              // 0x133 - 0x138

          public:
            void *GetSyncObjectFromRequest();
            void SetStopped(bool stopped);
            void SetSyncObjectStopReason(/*...*/ int reason);
        };
    } // namespace ue::game::humanai
} // namespace SDK
