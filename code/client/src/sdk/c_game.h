#pragma once

namespace SDK {
    class C_Player2;
    class C_Game {
        public:
            C_Player2* GetActivePlayer();
            void SetActivePlayer(C_Player2 *);

            bool IsGameUp();
    };

    C_Game* GetGame();
}