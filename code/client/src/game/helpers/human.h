#pragma once

#include <cstdint>

namespace SDK {
    class C_Human2;
}

namespace MafiaMP::Game::Helpers {
    class Human {
      public:
        static uint8_t GetHealthPercent(SDK::C_Human2 *);
    };
} // namespace MafiaMP::Game::Helpers
