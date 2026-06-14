#pragma once

#include "c_character_state_handler.h"
#include "../../c_second_order_filter.h"

namespace SDK {
    namespace ue::game::humanai {
        // Base class for all vehicle state handlers (Car, Boat, Train, Motorcycle)
        // Size: 384 bytes (0x180)
        class C_CharacterStateHandlerBaseVehicle : public C_CharacterStateHandler {
          protected:
            uint16_t m_unk64;                          // 0x40 - 0x42
            uint8_t m_unk66;                           // 0x42 - 0x43
            char _pad0[0x5];                           // 0x43 - 0x48
            uintptr_t m_unk72;                         // 0x48 - 0x50
            uintptr_t m_unk80;                         // 0x50 - 0x58
            char _pad1[0x8];                           // 0x58 - 0x60
            ue::C_SecondOrderFilter m_SteeringFilter;  // 0x60 - 0xC0
            ue::C_SecondOrderFilter m_ThrottleFilter;  // 0xC0 - 0x120
            uintptr_t m_unk288;                        // 0x120 - 0x128
            uintptr_t m_unk296;                        // 0x128 - 0x130
            uintptr_t m_unk304;                        // 0x130 - 0x138
            char _pad2[0x30];                          // 0x138 - 0x168
            uint32_t m_nDrivingFlags;                  // 0x168 - 0x16C
            bool m_bDrivingUnk;                        // 0x16C - 0x16D
            char _pad3[0x3];                           // 0x16D - 0x170
            uintptr_t m_unk368;                        // 0x170 - 0x178
            char _pad4[0x8];                           // 0x178 - 0x180

          public:
            void UpdateDrivingVariables(ue::sys::math::C_Vector const &, float, float, float, float);
            void UpdateMoodDescriptors();
        };
        static_assert(sizeof(C_CharacterStateHandlerBaseVehicle) == 0x180, "Wrong size: C_CharacterStateHandlerBaseVehicle");
    } // namespace ue::game::humanai
} // namespace SDK
