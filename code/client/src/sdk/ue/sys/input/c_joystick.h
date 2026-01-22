#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::sys::input {
        class C_Joystick {
          public:
            char pad0[0x08];                // 0x0000 - 0x0008
            int32_t m_deviceType;           // 0x0008 - 0x000C
            int32_t m_deviceId;             // 0x000C - 0x0010
            void *m_pDeviceInfo;            // 0x0010 - 0x0018
            char pad1[0x130];               // 0x0018 - 0x0148
            uint8_t m_buttonStates[16];     // 0x0148 - 0x0158
            char pad2[0x126];               // 0x0158 - 0x027E
            uint8_t m_buttonStatesAlt[16];  // 0x027E - 0x028E

          public:
            virtual ~C_Joystick() = default;

            bool IsButtonPressed(int buttonIndex) const {
                return (m_buttonStates[buttonIndex] & 0x80) != 0;
            }
        };
    } // namespace ue::sys::input
} // namespace SDK
