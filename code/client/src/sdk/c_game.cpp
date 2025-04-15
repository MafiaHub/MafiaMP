#include "c_game.h"
#include "patterns.h"

namespace SDK {
    C_Game *GetGame() {
        return hook::call<C_Game *>(gPatterns.C_Game__GetGame);
    }

    C_Player2 *C_Game::GetActivePlayer() {
        return m_pActivePlayer;
    }

    void C_Game::SetActivePlayer(C_Player2 *player) {
        m_pActivePlayer = player;
    }

    bool C_Game::IsGameUp() {
        return m_eGameUpState == E_GameUpState::UP;
    }
} // namespace SDK
