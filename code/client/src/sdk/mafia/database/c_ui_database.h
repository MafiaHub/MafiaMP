#pragma once

#include "sdk/ue/sys/sodb/c_database_interface.h"

namespace SDK {
    namespace mafia::database {
        class C_UIDatabase: public ue::sys::sodb::C_DatabaseInterface {
          public:
            class C_HUDTable {
              public:
                uint8_t padding_0[0x1E];
                bool m_RacingVisible = false;
                uint8_t padding_1[0x3]; // most likely other 'is visible' flags
                uint8_t padding_2[0x34A];
                float m_TargetTime          = 0.0f;
                uint16_t m_CurCheckpoint    = 0; // maybe
                uint16_t m_TotalCheckpoints = 0; // maybe
                uint16_t m_CurPosition      = 0;
                uint16_t m_TotalPositions   = 0;
                uint32_t m_Unknown          = 0;
                uint16_t m_CurLap           = 0;
                uint16_t m_TotalLaps        = 0;
                uint8_t m_Countdown         = 0;
            };

            C_HUDTable *GetHUDTable();

          private:
            uint8_t padding[0x20];
            C_HUDTable *m_HUDTable = nullptr;
        };
    }; // namespace mafia::framework
};     // namespace SDK
