#pragma once

#include "sdk/patterns.h"

#include <utils/hooking/hooking.h>

namespace SDK {
    namespace mafia::ui::hud {

        class C_RaceTimer {
          public:

            // This starts C_RaceManager!
            // If you do not wish to use C_RaceManager, use mafia::ui::hud::RaceXBin instead!
            void SetVisible(const bool visible) {
                hook::this_call(gPatterns.C_RaceTimer_SetVisible, this, visible);
            }

            // This starts C_RaceManager!
            // If you do not wish to use C_RaceManager, Use mafia::ui::hud::RaceXBin instead!
            void StartRace(const uint32_t numCheckpoints, const float targetTime, const uint32_t numLaps) {
                hook::this_call(gPatterns.C_RaceTimer_StartRace, this, numCheckpoints, targetTime, numLaps);
            }

          private:
            void *m_pVtable               = nullptr; // 0000 - 0008
            void *m_pUnk0                 = nullptr; // 0008 - 0010
            float m_fTimer                = 0.0f;    // 0010 - 0014
            uint32_t m_uCurrentCheckpoint = 0;       // 0014 - 0018
            uint32_t m_uCurrentLap        = 0;       // 0018 - 001C
        };

        class C_HudController {
          public:
            C_RaceTimer *GetRacingTimer() {
                return m_pRaceTimer;
            }

          private:
            char pad0[0x5A8];                       // 0000 - 05A8
            C_RaceTimer *m_pRaceTimer = nullptr;    // 05A8 - 05B0
        };
    } // namespace mafia::ui::hud
} // namespace SDK
