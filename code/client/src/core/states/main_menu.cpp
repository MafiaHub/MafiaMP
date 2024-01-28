#include "main_menu.h"
#include "states.h"

#include <utils/safe_win32.h>
#include <utils/states/machine.h>

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

        // Lock the game controls
        Game::Helpers::Controls::Lock(true);

        // Grab the view from the application
        auto const view = gApplication->GetWeb()->GetView(gApplication->GetMainMenuViewId());
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
            // TODO: do proper shutdown - this is just a quick hack
            // Notify the server, etc etc etc
            TerminateProcess(GetCurrentProcess(), 0);
        });

        return true;
    }

    bool MainMenuState::OnExit(Framework::Utils::States::Machine *) {
        // Grab the view from the application
        auto const view = gApplication->GetWeb()->GetView(gApplication->GetMainMenuViewId());
        if (!view) {
            return false;
        }

        // Unbind the event listeners
        view->RemoveEventListener("RUN_SANDBOX");
        view->RemoveEventListener("EXIT_APP");

        // Hide the view
        view->Display(false);
        view->Focus(false);

        // Unlock the game controls
        Game::Helpers::Controls::Lock(false);

        return true;
    }

    bool MainMenuState::OnUpdate(Framework::Utils::States::Machine *machine) {
        auto const view = gApplication->GetWeb()->GetView(gApplication->GetMainMenuViewId());
        if (!view) {
            return false;
        }

        /* gApplication->GetImGUI()->PushWidget([this]() {
            if (!ImGui::Begin("Debug", &_shouldDisplayWidget, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::End();
                return;
            }

            bool isDiscordPresent = gApplication->GetPresence()->IsInitialized();

            ImGui::Text("Enter connection details:");
            static char serverIp[32] = "127.0.0.1";
            static char nickname[32] = "Player";
            ImGui::Text("Server IP: ");
            ImGui::SameLine();
            ImGui::InputText("##server_ip", serverIp, 32);

            if (!isDiscordPresent) {
                ImGui::Text("Nickname: ");
                ImGui::SameLine();
                ImGui::InputText("##nickname", nickname, 32);
            }
            else {
                discord::User currUser {};
                gApplication->GetPresence()->GetUserManager().GetCurrentUser(&currUser);
                strcpy(nickname, currUser.GetUsername());
                ImGui::Text("Nickname: %s (set via Discord)", nickname);
            }

            if (ImGui::Button("Connect")) {
                // Update the application state for further usage
                Framework::Integrations::Client::CurrentState newApplicationState = MafiaMP::Core::gApplication->GetCurrentState();
                newApplicationState._host                                         = serverIp;
                newApplicationState._port                                         = 27015; // TODO: fix this
                newApplicationState._nickname                                     = nickname;
                MafiaMP::Core::gApplication->SetCurrentState(newApplicationState);

                // Request transition to next state (session connection)
                _shouldProceedConnection = true;
            }

            ImGui::SameLine();

            if (ImGui::Button("Play Offline (debug)")) {
                _shouldProceedOfflineDebug = true;
            }

            ImGui::End();
        });*/
        if (_shouldProceedOfflineDebug) {
            machine->RequestNextState(StateIds::SessionOfflineDebug);
        }
        /* if (_shouldProceedConnection) {
            machine->RequestNextState(StateIds::SessionConnection);
        }*/
        return /* _shouldProceedConnection || */_shouldProceedOfflineDebug;
    }
} // namespace MafiaMP::Core::States
