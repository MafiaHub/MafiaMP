#include "c_game.h"

#include "patterns.h"

namespace SDK {
    C_Game *GetGame() {
        return hook::call<C_Game *>(gPatterns.C_Game__GetGame);
    }

    C_Player2 *C_Game::GetActivePlayer() {
        // TODO: reverse I_Game
        const auto vtableToAddr = *(uint64_t *)(*(uintptr_t *)(this) + 0x90);
        return reinterpret_cast<C_Player2 *>(hook::this_call<uint64_t>(vtableToAddr, this));
    }

    void C_Game::SetActivePlayer(C_Player2 *player) {
        const auto vtableToAddr = *(uint64_t *)(*(uintptr_t *)(this) + 0xA0);
        return hook::this_call(vtableToAddr, this, player);
    }

    bool C_Game::IsGameUp() {
        const auto vtableToAddr = *(uint64_t *)(*(uintptr_t *)(this) + 0x100);
        return hook::this_call<bool>(vtableToAddr, this);
    }
} // namespace SDK