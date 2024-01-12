#pragma once

#include <stdint.h>

namespace SDK {
    class C_Player2;
}

namespace MafiaMP::Game::Helpers {
    class Controls {
      public:
        static SDK::C_Player2 *GetLocalPlayer();
        static void Lock(bool);
        static bool AreControlsLocked();
        static uint64_t GetLocalPlayerSpawnProfile();
        static void PlayerChangeSpawnProfile(uint64_t spawnProfile);
    };
} // namespace MafiaMP::Game::Helpers
