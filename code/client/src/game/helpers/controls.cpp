#include "controls.h"

#include "../../sdk/mafia/framework/c_mafia_framework_interfaces.h"
#include "../../sdk/mafia/framework/c_mafia_framework.h"

#include "../../sdk/c_game_input_module.h"
#include "../../sdk/entities/c_player_2.h"

namespace MafiaMP::Game::Helpers {
    SDK::C_Player2* Controls::GetLocalPlayer() {
        SDK::mafia::framework::C_MafiaFramework *pMafiaFramework = SDK::mafia::framework::C_MafiaFramework::GetInstance();
        if (!pMafiaFramework) {
            return nullptr;
        }

        SDK::mafia::framework::C_MafiaFrameworkInterfaces *pMafiaFrameworkInterfaces = pMafiaFramework->GetInterfaces();
        if (!pMafiaFrameworkInterfaces) {
            return nullptr;
        }
        SDK::C_Game *pGame = pMafiaFrameworkInterfaces->GetGame();
        if (!pGame) {
            return nullptr;
        }

        SDK::C_Player2 *pActivePlayer = (SDK::C_Player2 *)(pGame->IsGameUp() ? pGame->GetActivePlayer() : nullptr);
        if (!pActivePlayer) {
            return nullptr;
        }
        return pActivePlayer;
    }

    void Controls::Lock(bool lock) {
        auto pActivePlayer = GetLocalPlayer();
        if (!pActivePlayer) {
            return;
        }

        if (lock) {
            pActivePlayer->GetCharacterController()->LockControls();
        }
        else {
            pActivePlayer->GetCharacterController()->UnlockControls();
        }
        SDK::GetGameInputModule()->PauseInput(lock);
    }
}
