#pragma once

#include "c_character_state_handler_base_vehicle.h"

namespace SDK {
    namespace ue::game::humanai {
        // Size: ~448 bytes (0x1C0)
        class C_CharacterStateHandlerTrain : public C_CharacterStateHandlerBaseVehicle {
          protected:
            uintptr_t m_unk384;           // 0x180 - 0x188
            int32_t m_nTrainState;        // 0x188 - 0x18C (default 8)
            int32_t m_nTrainMode;         // 0x18C - 0x190 (default 8)
            uintptr_t m_unk400;           // 0x190 - 0x198
            bool m_bTrainUnk;             // 0x198 - 0x199
            char _pad0[0x7];              // 0x199 - 0x1A0
            int64_t m_nTrainTarget;       // 0x1A0 - 0x1A8 (default -1)
            char _pad1[0x8];              // 0x1A8 - 0x1B0
            bool m_bTrainUnk2;            // 0x1B0 - 0x1B1
            char _pad2[0x7];              // 0x1B1 - 0x1B8
            uintptr_t m_unk440;           // 0x1B8 - 0x1C0
        };
    } // namespace ue::game::humanai
} // namespace SDK
