#pragma once

#include <stdint.h>

namespace SDK {
    namespace mafia::ui::hud {

        /**
         * Handy structure which provides an API to update the Racing HUD Element.
         * Note that there are some discrepancies with the executable code however
         * all functionality should be the same.
         */
        class RaceXBin {
          public:

            /**
             * Set the visibility of the Racing HUD Element.
             * @param bVisibility - Should the HUD Element be visible
             */
            static void SetVisible(const bool bVisiblity);

            /**
             * Set the Target Time on the Racing HUD Element.
             * Note that this may not have an effect (eg. appear on the HUD)
             * @param InTargetTime - Target Time to show on the HUD
             */
            static void SetTargetTime(const float InTargetTime);

            /**
             * Set the current Position on the Racing HUD Element.
             * Note that 'Total Positions' may need to be set before this to have an effect.
             * @param InPosition - The current position out of Total Positions
             */
            static void SetPosition(const uint16_t InPosition);

            /**
             * Set the max amount of Positions on the Racing HUD Element
             * @param InTotalPosition - Max number of positions
             */
            static void SetPositionTotal(const uint16_t InTotalPosition);

            /**
             * Set the current Lap on the Racing HUD Element.
             * Note that 'Total Laps' may need to be set before this to have an effect.
             * @param InLaps - The current Lap out of Total Laps
             */
            static void SetLaps(const uint16_t InLaps);

            /**
             * Set the number of Laps on the Racing HUD Element
             * @param InTotalPosition - Number of Laps
             */
            static void SetLapsTotal(const uint16_t InTotalLaps);

            /**
             * Set the current Checkpoint on the Racing HUD Element.
             * Note that 'Total Checkpoints' may need to be set before this to have an effect.
             * @param InPosition - The current Checkpoint out of Total Checkpoints
             */
            static void SetCheckpoints(const uint16_t InCheckpoint);

            /**
             * Set the max amount of Checkpoints on the Racing HUD Element
             * @param InTotalPosition - Max number of checkpoints
             */
            static void SetCheckpointsTotal(const uint16_t InTotalCheckpoints);

            /**
             * Set the countdown. Max is 3, Minimum is 0.
             * The HUD automatically plays the noise when the countdown is updated.
             * The caller will have to manage a timer for the countdown.
             * @param InCountdown - Current step in the Countdown
             */
            static void SetCountdown(const uint8_t InCountdown);
        };
    } // namespace mafia::ui::hud
} // namespace SDK
