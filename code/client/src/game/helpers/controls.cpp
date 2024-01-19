#include "controls.h"

#include "sdk/mafia/framework/c_mafia_framework_interfaces.h"

#include "sdk/mafia/framework/c_mafia_framework.h"

#include "sdk/c_game_input_module.h"
#include "sdk/entities/c_player_2.h"

namespace MafiaMP::Game::Helpers {
    SDK::C_Player2 *Controls::GetLocalPlayer() {
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

    bool Controls::AreControlsLocked() {
        auto pActivePlayer = GetLocalPlayer();
        if (!pActivePlayer) {
            return false;
        }

        return pActivePlayer->GetCharacterController()->AreControlsLocked();
    }

    uint64_t Controls::GetLocalPlayerSpawnProfile() {
        // TODO fixme
        SDK::mafia::framework::C_MafiaFramework *mafiaFramework = SDK::mafia::framework::C_MafiaFramework::GetInstance();
        if (!mafiaFramework) {
            return 0;
        }

        SDK::mafia::framework::C_MafiaFrameworkInterfaces *mafiaFrameworkInterfaces = mafiaFramework->GetInterfaces();
        if (!mafiaFrameworkInterfaces) {
            return 0;
        }
        SDK::C_PlayerModelManager *playerModelManager = mafiaFrameworkInterfaces->GetPlayerModelManager();
        if (!playerModelManager) {
            return 0;
        }

        SDK::C_PlayerSpawner *playerSpawner = playerModelManager->GetPlayerSpawner();
        if (!playerSpawner) {
            return 0;
        }

        return playerSpawner->GetSpawnProfile();
    }

    void Controls::PlayerChangeSpawnProfile(uint64_t spawnProfile) {
        // TODO fixme
        SDK::mafia::framework::C_MafiaFramework *mafiaFramework = SDK::mafia::framework::C_MafiaFramework::GetInstance();
        if (!mafiaFramework) {
            return;
        }

        SDK::mafia::framework::C_MafiaFrameworkInterfaces *mafiaFrameworkInterfaces = mafiaFramework->GetInterfaces();
        if (!mafiaFrameworkInterfaces) {
            return;
        }
        SDK::C_PlayerModelManager *playerModelManager = mafiaFrameworkInterfaces->GetPlayerModelManager();
        if (!playerModelManager) {
            return;
        }

        playerModelManager->SwitchSpawnProfile(spawnProfile);
    }
} // namespace MafiaMP::Game::Helpers
