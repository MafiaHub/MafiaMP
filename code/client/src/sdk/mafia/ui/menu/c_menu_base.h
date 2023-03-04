#pragma once

#include <cstdint>

namespace SDK {
    namespace mafia::ui::menu {
        class C_MenuBase {
          public:
            enum class E_ScriptedMenuEvent : uint8_t {
                ACCEPT = 0,
                BACK = 1,
                ACCEPT_X = 2,
                ACCEPT_Y = 3,
                VIEWED = 4,
                CLEAR_MESSAGES_QUEUE = 5,
                ALLOW_X_INPUT = 6,
                CREDITS_FINISHED = 7,
                UNK_TICK = 8,
                UNK_LOADING = 12,
                UNK_LOADING_2 = 13
            };
        };
    } // namespace mafia::ui::menu
} // namespace SDK
