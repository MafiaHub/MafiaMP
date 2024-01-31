#pragma once

#include "c_entity_indicator_controller.h"

namespace SDK {
    namespace mafia::ui::hud {
        class C_HudController {
          public:
            char pad0[0x160];                                           // 0000 - 0160
            C_EntityIndicatorController *m_entityIndicatorController;   // 0160 - 0168
        };
    } // namespace mafia::ui::hud
} // namespace SDK
