#include "main_menu.h"
#include "states.h"

#include <utils/safe_win32.h>
#include <utils/states/machine.h>

#include <nlohmann/json.hpp>
#include <string>

#include "../../game/helpers/controls.h"

#include "../application.h"

namespace MafiaMP::Core::States {
    MainMenuState::MainMenuState() {}

    MainMenuState::~MainMenuState() {}

    int32_t MainMenuState::GetId() const {
        return StateIds::MainMenu;
    }

    const char *MainMenuState::GetName() const {
        return "MainMenu";
    }

    bool MainMenuState::OnEnter(Framework::Utils::States::Machine *) {
        // Reset the states
        _shouldProceedOfflineDebug = false;
        _shouldProceedConnection   = false;

        // Lock game controls
        gApplication->LockControls(true);

        // Grab the view from the application
        auto const view = gApplication->GetWebManager()->GetView(gApplication->GetMainMenuViewId());
        if (!view) {
            return false;
        }

        view->Display(true);
        view->Focus(true);

        // Bind the event listeners
        view->AddEventListener("RUN_SANDBOX", [this](std::string eventPayload) {
            _shouldProceedOfflineDebug = true;
        });

        view->AddEventListener("EXIT_APP", [this](std::string eventPayload) {
            TerminateProcess(GetCurrentProcess(), 0);
        });

        view->AddEventListener("CONNECT_TO_HOST", [this](std::string eventPayload) {
            auto const parsedPayload = nlohmann::json::parse(eventPayload);

            // Make sure the payload is valid
            if (!parsedPayload.count("host") || !parsedPayload.count("port")) {
                Framework::Logging::GetLogger("WebManager")->critical("Invalid payload for CONNECT_TO_HOST event");
                return;
            }

            // Make sure the port is valid
            if (!parsedPayload["port"].is_number()) {
                Framework::Logging::GetLogger("WebManager")->critical("Invalid port for CONNECT_TO_HOST event");
                return;
            }

            // Make sure the host is valid
            if (!parsedPayload["host"].is_string() || parsedPayload["host"].get<std::string>().empty()) {
                Framework::Logging::GetLogger("WebManager")->critical("Invalid host for CONNECT_TO_HOST event");
                return;
            }

            // Update the application state for further usage
            Framework::Integrations::Client::CurrentState newApplicationState = gApplication->GetCurrentState();
            newApplicationState._host                                         = parsedPayload["host"];
            newApplicationState._port                                         = parsedPayload["port"];
            newApplicationState._nickname                                     = "Player";
            if (gApplication->GetPresence()->IsInitialized()) {
                discord::User currUser {};
                gApplication->GetPresence()->GetUserManager().GetCurrentUser(&currUser);
                newApplicationState._nickname = currUser.GetUsername();
            }
            gApplication->SetCurrentState(newApplicationState);

            // Request transition to next state (session connection)
            _shouldProceedConnection = true;
        });
        return true;
    }

    bool MainMenuState::OnExit(Framework::Utils::States::Machine *) {
        // Grab the view from the application
        auto const view = gApplication->GetWebManager()->GetView(gApplication->GetMainMenuViewId());
        if (!view) {
            return false;
        }

        // Unbind the event listeners
        view->RemoveEventListener("RUN_SANDBOX");
        view->RemoveEventListener("CONNECT_TO_HOST");
        view->RemoveEventListener("EXIT_APP");

        // Hide the view
        view->Display(false);
        view->Focus(false);

        // Unlock the game controls
        Game::Helpers::Controls::Lock(false);

        gApplication->LockControls(false);
        return true;
    }

    bool MainMenuState::OnUpdate(Framework::Utils::States::Machine *machine) {
        auto const view = gApplication->GetWebManager()->GetView(gApplication->GetMainMenuViewId());
        if (!view) {
            return false;
        }

        if (_shouldProceedOfflineDebug) {
            machine->RequestNextState(StateIds::SessionOfflineDebug);
        }
        if (_shouldProceedConnection) {
            machine->RequestNextState(StateIds::SessionConnection);
        }
        return _shouldProceedConnection || _shouldProceedOfflineDebug;
    }
} // namespace MafiaMP::Core::States
