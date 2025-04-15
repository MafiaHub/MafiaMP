#include "game_setup.h"

#include "states.h"

#include "sdk/ue/game/traffic/c_streaming_traffic_module.h"
#include "sdk/ue/gfx/environmenteffects/c_gfx_environment_effects.h"

#include <utils/states/machine.h>

namespace MafiaMP::Core::States {
    GameSetupState::GameSetupState() {}

    GameSetupState::~GameSetupState() {}

    int32_t GameSetupState::GetId() const {
        return StateIds::GameReady;
    }

    const char *GameSetupState::GetName() const {
        return "GameSetup";
    }

    bool GameSetupState::OnEnter(Framework::Utils::States::Machine *) {
        return true;
    }

    bool GameSetupState::OnExit(Framework::Utils::States::Machine *machine) {
        machine->RequestNextState(StateIds::MainMenu);
        return true;
    }

    bool GameSetupState::OnUpdate(Framework::Utils::States::Machine *) {
        // Close the season (disable traffic)
        {
            const auto streamingTrafficModule = SDK::ue::game::traffic::C_StreamingTrafficModule::GetInstance();
            if (streamingTrafficModule) {
                streamingTrafficModule->CloseSeason(true);
            }
            else {
                Framework::Logging::GetLogger("game")->warn("GameSetupState: C_StreamingTrafficModule is null");
            }
        }

        // Set the time flow multiplier to 0 (so we have control over the time)
        {
            const auto gfxEnvironmentEffects = SDK::ue::gfx::environmenteffects::C_GfxEnvironmentEffects::GetInstance();
            if (gfxEnvironmentEffects) {
                const auto weatherManager = gfxEnvironmentEffects->GetWeatherManager();
                if (weatherManager) {
                    weatherManager->SetUserTimeFlowSpeedMult(0);
                }
                else {
                    Framework::Logging::GetLogger("game")->warn("GameSetupState: C_GfxWeatherManager is null");
                }
            }
            else {
                Framework::Logging::GetLogger("game")->warn("GameSetupState: C_GfxEnvironmentEffects is null");
            }
        }
        return true;
    }
} // namespace MafiaMP::Core::States
