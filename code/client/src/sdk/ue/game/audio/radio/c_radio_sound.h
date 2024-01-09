#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::game::audio::radio {

        class C_BroadcastStation {
          public:

              // ID correlates with E_RadioStation
              uint32_t GetStationID() const {
                  return m_uID;
              }

          private:

            char pad_0[0x5C];

            uint32_t m_uID = 0; // ID correlates with E_RadioStation; 0x5C
        };

        class C_RadioSound {
          public:

            char pad_0[0x28];
            int8_t m_iStationID   = 0; // 0x28 (assume its E_RadioStation)
            int8_t m_iUnk1        = 0; // 0x29
            int8_t m_iNumSwitches = 0; // increments each time the station is changed; 0x2A
            int8_t m_iUnk2        = 0; // 2B
            char pad_2C[0x2C];
            class C_BroadcastStation *m_BroadcastStation = nullptr; // 0x58

            uint32_t GetCurrentStation() const {
                return m_BroadcastStation->GetStationID();
            }
        };
    }
}
