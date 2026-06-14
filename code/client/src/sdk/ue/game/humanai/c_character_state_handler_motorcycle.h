#pragma once

#include "c_character_state_handler_base_vehicle.h"
#include "../../sys/math/c_vector.h"

namespace SDK {
    namespace ue::game::humanai {
        // Size: ~520 bytes (0x208)
        class C_CharacterStateHandlerMotorcycle : public C_CharacterStateHandlerBaseVehicle {
          protected:
            uintptr_t m_unk384;                    // 0x180 - 0x188
            uintptr_t m_unk392;                    // 0x188 - 0x190
            uintptr_t m_unk400;                    // 0x190 - 0x198
            bool m_bMotorcycleUnk;                 // 0x198 - 0x199
            char _pad0[0x3];                       // 0x199 - 0x19C
            ue::sys::math::C_Vector m_LeanVector;  // 0x19C - 0x1A8 (default zero)
            int32_t m_nMotorcycleState;            // 0x1A8 - 0x1AC (default 13)
            int32_t m_nMotorcycleMode;             // 0x1AC - 0x1B0 (default 13)
            char m_LeanFilter[0x24];               // 0x1B0 - 0x1D4 (internal filter data)
            uintptr_t m_unk468;                    // 0x1D4 - 0x1DC
            uintptr_t m_unk476;                    // 0x1DC - 0x1E4
            uintptr_t m_unk484;                    // 0x1E4 - 0x1EC
            uintptr_t m_unk492;                    // 0x1EC - 0x1F4
            bool m_bIsActive;                      // 0x1F4 - 0x1F5
            char _pad1[0x3];                       // 0x1F5 - 0x1F8
            uintptr_t m_unk504;                    // 0x1F8 - 0x200
            bool m_bMotorcycleUnk2;                // 0x200 - 0x201
            char _pad2[0x7];                       // 0x201 - 0x208
        };
    } // namespace ue::game::humanai
} // namespace SDK
