#pragma once

#include <utils/hooking/hooking.h>

namespace SDK {
    namespace mafia::ui::hud {

        class C_RaceTimer {
          public:
            void SetVisible(const bool visible) {
                hook::this_call(0x14304F520, this, visible); // Set RaceTimer visible
                hook::this_call(0x14305BF80, this, 20, 500.0f, 10); // Start Race

            }
        };

        class C_HudController {
          public:
            C_RaceTimer *GetRacingTimer() {
                return hook::this_call<C_RaceTimer *>(0x142FE8AD0, this);
            }
        };
    } // namespace mafia::ui::hud
} // namespace SDK
