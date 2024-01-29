#pragma once

#include <utils/hooking/hooking.h>

#include <stdint.h>

namespace SDK {
    namespace mafia::ui::hud {
        class RaceXBin {
          public:
            // NB: This likely won't have an effect; use C_RaceTimer instead
            static void SetVisible(bool visibility) {
                hook::call<bool>(0x14304F6C0, visibility);
            }

            static void SetTargetTime(float time) {
                // NB: for some reason causes compile error
                // hook::call<float>(0x14304DA00, time);
            }

            static void SetPosition(uint16_t pos) {
                hook::call<uint16_t>(0x1430496F0, pos);
            }

            static void SetPositionTotal(uint16_t pos) {
                hook::call<uint16_t>(0x143049820, pos);
            }

            static void SetLaps(uint16_t laps) {
                hook::call<uint16_t>(0x143047620, laps);
            }

            static void SetLapsTotal(uint16_t laps) {
                hook::call<uint16_t>(0x143047750, laps);
            }

            static void SetCheckpoints(uint16_t checkpoint) {
                hook::call<uint16_t>(0x143042300, checkpoint);
            }

            static void SetCheckpointsTotal(uint16_t checkpoint) {
                hook::call<uint16_t>(0x143042430, checkpoint);
            }

            static void SetCountdown(uint8_t countdown) {
                hook::call<uint8_t>(0x143042670, countdown);
            }
        };
    } // namespace mafia::ui::hud
} // namespace SDK
