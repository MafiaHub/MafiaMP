#pragma once

#include <utils/hooking/hooking.h>

namespace SDK {
    namespace mafia::ui::hud {

        class C_RaceTimer {
          public:
            void SetVisible(const bool visible) {
                hook::this_call(0x14304F520, this, visible);
            }

            void StartRace(const uint32_t NumCheckpoints, const float TargetTime, const uint32_t NumLaps) {
                hook::this_call(0x14305BF80, this, NumCheckpoints, TargetTime, NumLaps);
            }

            void NextLap() {
                m_CurrentCheckpoint = 0;
                m_CurrentLap++;
            }

            uint32_t GetCurrentCheckpoint() {
                return m_CurrentCheckpoint;
            }

            uint32_t GetCurrentLap() {
                return m_CurrentLap;
            }

          private:
            void *m_Vtable               = nullptr;
            void *m_Unk0                 = nullptr;
            float m_Timer                = 0.0f;
            uint32_t m_CurrentCheckpoint = 0;
            uint32_t m_CurrentLap        = 0;
        };

        class C_HudController {
          public:
            char pad0[0x5A8]; // 0000 - 0x5A8
            C_RaceTimer *m_RaceTimer = nullptr;

            C_RaceTimer *GetRacingTimer() {
                return m_RaceTimer;
            }
        };
    } // namespace mafia::ui::hud
} // namespace SDK
