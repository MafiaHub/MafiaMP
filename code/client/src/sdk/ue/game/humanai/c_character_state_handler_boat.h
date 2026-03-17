#pragma once

#include "c_character_state_handler_base_vehicle.h"

namespace SDK {
    namespace ue::game::humanai {
        // Size: ~488 bytes (0x1E8)
        class C_CharacterStateHandlerBoat : public C_CharacterStateHandlerBaseVehicle {
          protected:
            uintptr_t m_unk384;           // 0x180 - 0x188
            uintptr_t m_unk392;           // 0x188 - 0x190
            uintptr_t m_unk400;           // 0x190 - 0x198
            uint32_t m_nBoatFlags;        // 0x198 - 0x19C
            int32_t m_nSeatIndex;         // 0x19C - 0x1A0 (default -1)
            bool m_bBoatUnk;              // 0x1A0 - 0x1A1
            char _pad0[0x3];              // 0x1A1 - 0x1A4
            uintptr_t m_unk420;           // 0x1A4 - 0x1AC
            char _pad1[0x4];              // 0x1AC - 0x1B0
            uintptr_t m_unk432;           // 0x1B0 - 0x1B8
            uintptr_t m_unk440;           // 0x1B8 - 0x1C0
            uintptr_t m_unk448;           // 0x1C0 - 0x1C8
            uintptr_t m_unk456;           // 0x1C8 - 0x1D0
            uintptr_t m_unk464;           // 0x1D0 - 0x1D8
            uintptr_t m_unk472;           // 0x1D8 - 0x1E0
            uintptr_t m_unk480;           // 0x1E0 - 0x1E8

          public:
            void OnStartRagdoll();
        };
    } // namespace ue::game::humanai
} // namespace SDK
