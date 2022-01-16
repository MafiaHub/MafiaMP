#pragma once

namespace SDK {
    class C_Player2;
}

namespace MafiaMP::Game::Helpers {
    class Controls {
      public:
        static SDK::C_Player2 *GetLocalPlayer();
        static void Lock(bool);
    };
}
