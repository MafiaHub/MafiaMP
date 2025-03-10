#pragma once

#include <cstdint>

#include "sdk/c_ticked_module.h"
#include "ue/c_mafia_game_up_context.h"
#include "sdk/ue/game/framework/c_game_context.h"

namespace SDK {
    class C_Player2;

    class I_Game: public C_TickedModule {
      public:
        enum class E_GameUpState {
            UP = 2,
        };

        public:
            virtual ~I_Game() = default;
    };

    class C_Game : public I_Game {
        public:
            ue::C_MafiaGameUpContext *m_pGameUpContext;         // 0008 - 0010
            char pad0[0x38];                                    // 0010 - 0048
            ue::game::framework::C_GameContext *m_pGameContext; // 0048 - 0050
            char pad2[0x30];                                    // 0050 - 0080
            float m_fPlayerSpeedCff;                            // 0080 - 0084
            int32_t m_iTimeFromStart;                           // 0084 - 0088
            uint64_t m_iPlayTime;                               // 0088 - 0090
            I_Game::E_GameUpState m_eGameUpState;               // 0090 - 0094
            char pad4[0x4];                                     // 0094 - 0098
            C_Player2 *m_pActivePlayer;                         // 0098 - 00A0

        public:
            C_Player2* GetActivePlayer();
            void SetActivePlayer(C_Player2 *);

            bool IsGameUp();
    };

    C_Game* GetGame();
}
