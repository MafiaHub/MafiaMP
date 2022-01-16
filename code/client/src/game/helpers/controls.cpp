#include "controls.h"

#include "../../sdk/mafia/framework/c_mafia_framework_interfaces.h"
#include "../../sdk/mafia/framework/c_mafia_framework.h"

#include "../../sdk/c_game_input_module.h"
#include "../../sdk/entities/c_player_2.h"

namespace MafiaMP::Game::Helpers {
    void Controls::Lock(bool lock) {
        SDK::mafia::framework::C_MafiaFramework *pMafiaFramework = SDK::mafia::framework::C_MafiaFramework::GetInstance();
        if (!pMafiaFramework) {
            return;
        }

        SDK::mafia::framework::C_MafiaFrameworkInterfaces *pMafiaFrameworkInterfaces = pMafiaFramework->GetInterfaces();
        if (!pMafiaFrameworkInterfaces) {
            return;
        }
        SDK::C_Game *pGame = pMafiaFrameworkInterfaces->GetGame();
        if (!pGame) {
            return;
        }

        SDK::C_Player2 *pActivePlayer = (SDK::C_Player2 *)(pGame->IsGameUp() ? pGame->GetActivePlayer() : nullptr);
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
