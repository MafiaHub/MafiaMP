#pragma once

#include "../../ue/sys/sodb/c_database_interface.h"

namespace SDK {
    namespace mafia::database {
        class C_UIDatabase: public ue::sys::sodb::C_DatabaseInterface {
          public:
            class C_HUDTable {
              public:
                uint8_t pad0[0x1E];                         // 0000 - 001E
                bool m_bRacingVisible           = false;    // 001E - 001F
                uint8_t pad1[0x3];                          // 001F - 0022
                uint8_t pad2[0x346];                        // 0022 - 0368
                float m_fElapsedTime            = 0.0f;     // 0368 - 036C
                float m_fTargetTime             = 0.0f;     // 036C - 0370
                uint16_t m_uCurCheckpoint       = 0;        // 0370 - 0372
                uint16_t m_uTotalCheckpoints    = 0;        // 0372 - 0374
                uint16_t m_uCurPosition         = 0;        // 0374 - 0376
                uint16_t m_uTotalPositions      = 0;        // 0376 - 0378
                uint16_t m_uUnknown1            = 0;        // 0378 - 037C
                uint16_t m_uUnknown2            = 0;        // 0378 - 037C
                uint16_t m_uCurLap              = 0;        // 037C - 037E
                uint16_t m_uTotalLaps           = 0;        // 037E - 0380
                uint8_t m_uCountdown            = 0;        // 0380 - 0381
            };

            C_HUDTable *GetHUDTable() const {
                return m_pHUDTable;
            }

          private:
            // NB: Atleast 0x38 is part of C_DatabaseInterface
            // Since m_pHUDTable is at 0x20, that could actually be part of base class
            uint8_t pad0[0x20];                                   // 0000 - 0020
            C_HUDTable *m_pHUDTable             = nullptr;        // 0020 - 0028
        };
    }; // namespace mafia::database
};     // namespace SDK
