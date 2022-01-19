#include <utils/safe_win32.h>
#include "menu.h"
#include "states.h"

#include <utils/states/machine.h>

#include <imgui/imgui.h>

#include "../../game/helpers/controls.h"
#include "../../game/helpers/camera.h"
#include "../../sdk/mafia/framework/c_mafia_framework_interfaces.h"
#include "../../sdk/mafia/framework/c_mafia_framework.h"

#include "../application.h"

namespace MafiaMP::Core::States {
    InMenuState::InMenuState() {}

    InMenuState::~InMenuState() {}

    int32_t InMenuState::GetId() const {
        return StateIds::Menu;
    }

    const char *InMenuState::GetName() const {
        return "InMenu";
    }

    bool InMenuState::OnEnter(Framework::Utils::States::Machine *) {
        _shouldDisplayWidget = true;
        _shouldProceedConnection = false;
        _shouldProceedOfflineDebug = false;

        // Set camera
        Game::Helpers::Camera::SetPos({450.43698, -646.01941, 58.132675}, {-399.2962, -594.75391, 37.324718}, true);

        // Lock game controls
        Game::Helpers::Controls::Lock(true);

        // Enable cursor
        gApplication->GetImGUI()->ShowCursor(true);
        return true;
    }

    bool InMenuState::OnExit(Framework::Utils::States::Machine *) {
        // Temp
        Game::Helpers::Camera::ResetBehindPlayer();

        // Hide cursor
        gApplication->GetImGUI()->ShowCursor(false);
        return true;
    }

    bool InMenuState::OnUpdate(Framework::Utils::States::Machine *machine) {
        gApplication->GetImGUI()->PushWidget([this]() {
            if (!ImGui::Begin("Debug", &_shouldDisplayWidget)) {
                ImGui::End();
                return;
            }

            ImGui::Text("Enter connection details:");
            static char serverIp[32] = "127.0.0.1";
            static char nickname[32] = "Player";
            ImGui::Text("Server IP: ");
            ImGui::SameLine();
            ImGui::InputText("##server_ip", serverIp, 32);
            ImGui::Text("Nickname: ");
            ImGui::SameLine();
            ImGui::InputText("##nickname", nickname, 32);
            if (ImGui::Button("Connect")) {
                // Update the application state for further usage
                Framework::Integrations::Client::CurrentState newApplicationState;
                newApplicationState._host = serverIp;
                newApplicationState._port     = 27015; //TODO: fix this
                newApplicationState._nickname = nickname;
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
