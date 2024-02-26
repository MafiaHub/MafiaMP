#pragma once

#include "sdk/ue/sys/sodb/c_database_interface.h"

namespace SDK {
    namespace mafia::database {
        class C_UIDatabase: public ue::sys::sodb::C_DatabaseInterface {
          public:
            class C_HUDTable {
              public:
                // char padding_0[0x16];
                // uint32_t m_RacingVisible = 0;
                uint8_t padding[0x36C];
                float m_TargetTime          = 0.0f;
                uint16_t m_CurPosition      = 0;
                uint16_t m_TotalPositions   = 0;
                uint16_t m_CurCheckpoint    = 0;
                uint16_t m_TotalCheckpoints = 0;
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
