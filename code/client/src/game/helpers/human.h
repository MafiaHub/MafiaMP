#pragma once

#include <cstdint>

namespace SDK {
    class C_Human2;
    class C_Car;
}

namespace MafiaMP::Game::Overrides {
    class CharacterController;
}


namespace MafiaMP::Game::Helpers {
    class Human {
      public:
        static uint8_t GetHealthPercent(SDK::C_Human2 *);
        static void SetHealthPercent(SDK::C_Human2 *, float);
        static bool PutIntoCar(MafiaMP::Game::Overrides::CharacterController *, SDK::C_Car *, int, bool);
        static bool RemoveFromCar(MafiaMP::Game::Overrides::CharacterController *, SDK::C_Car *, bool);
        static void AddWeapon(SDK::C_Human2 *, int, int);
    };
} // namespace MafiaMP::Game::Helpers
