#include "main_menu.h"
#include "states.h"
#include <utils/safe_win32.h>

#include <utils/states/machine.h>

#include <imgui/imgui.h>

#include "../../game/helpers/camera.h"
#include "../../game/helpers/controls.h"

#include "sdk/mafia/framework/c_mafia_framework.h"
#include "sdk/mafia/framework/c_mafia_framework_interfaces.h"

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
        _shouldDisplayWidget       = true;
        _shouldProceedConnection   = false;
        _shouldProceedOfflineDebug = false;

        // Set camera
        Game::Helpers::Camera::SetPos({-986.40686, -304.061798, 2.292042}, {-985.365356, -336.348083, 2.892426}, true);

        // Lock game controls
        gApplication->LockControls(true);
        return true;
    }

    bool MainMenuState::OnExit(Framework::Utils::States::Machine *) {
        // Temp
        Game::Helpers::Camera::ResetBehindPlayer();

        gApplication->LockControls(false);
        return true;
    }

    bool MainMenuState::OnUpdate(Framework::Utils::States::Machine *machine) {
        gApplication->GetImGUI()->PushWidget([this]() {
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
        });
        if (_shouldProceedOfflineDebug) {
            machine->RequestNextState(StateIds::SessionOfflineDebug);
        }
        if (_shouldProceedConnection) {
            machine->RequestNextState(StateIds::SessionConnection);
        }
        return _shouldProceedConnection || _shouldProceedOfflineDebug;
    }
} // namespace MafiaMP::Core::States
