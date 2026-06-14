#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::sys::input {
        class C_Device {
          public:
            char pad0[0x08];                // 0x0000 - 0x0008
            int32_t m_deviceType;           // 0x0008 - 0x000C
            int32_t m_deviceId;             // 0x000C - 0x0010
            void *m_pDeviceInterface;       // 0x0010 - 0x0018

          public:
            virtual ~C_Device() = default;

            int32_t GetDeviceType() const {
                return m_deviceType;
            }
        };
    } // namespace ue::sys::input
} // namespace SDK
