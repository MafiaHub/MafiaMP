#include "game_ready.h"

#include "states.h"

#include "sdk/ue/game/traffic/c_streaming_traffic_module.h"
#include "sdk/ue/gfx/environmenteffects/c_gfx_environment_effects.h"

#include <utils/states/machine.h>

namespace MafiaMP::Core::States {
    GameReadyState::GameReadyState() {}

    GameReadyState::~GameReadyState() {}

    int32_t GameReadyState::GetId() const {
        return StateIds::GameReady;
    }

    const char *GameReadyState::GetName() const {
        return "GameReady";
    }

    bool GameReadyState::OnEnter(Framework::Utils::States::Machine *) {
        return true;
    }

    bool GameReadyState::OnExit(Framework::Utils::States::Machine *machine) {
        machine->RequestNextState(StateIds::MainMenu);
        return true;
    }

    bool GameReadyState::OnUpdate(Framework::Utils::States::Machine *) {
        const auto streamingTrafficModule = SDK::ue::game::traffic::C_StreamingTrafficModule::GetInstance();
        if (!streamingTrafficModule) {
            return false;
        }

        const auto gfxEnvironmentEffects = SDK::ue::gfx::environmenteffects::C_GfxEnvironmentEffects::GetInstance();
        if (!gfxEnvironmentEffects) {
            return false;
        }
        const auto weatherManager = gfxEnvironmentEffects->GetWeatherManager();
        if (!weatherManager) {
            return false;
        }

        /**
         * Disable traffic
         *
         * Traffic is automaticaly loaded by the game via C_StreamingTrafficModule::OpenSeason
         * after C_StreamMap::OpenPart("freeride") is called.
         *
         * We close the season here which disable the traffic.
         */
        streamingTrafficModule->CloseSeason(true);

        /**
         * Disable TimeFlow
         *
         * We give scripters the option to update the time manually.
         */
        weatherManager->SetUserTimeFlowSpeedMult(0);

        return true;
    }
} // namespace MafiaMP::Core::States
