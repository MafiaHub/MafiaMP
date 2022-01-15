#include "menu.h"
#include "states.h"

#include <utils/states/machine.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx11.h>

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
        return true;
    }

    bool InMenuState::OnExit(Framework::Utils::States::Machine *) {
        return true;
    }

    bool InMenuState::OnUpdate(Framework::Utils::States::Machine *) {
        static bool shouldExit = false;
        gApplication->GetImGUI()->PushWidget([&]() {
            static bool open = true;
            if (!ImGui::Begin("Debug", &open)) {
                ImGui::End();
                shouldExit = true;
            }

            ImGui::Text("Press F1 to turn this window on/off.");
            if (ImGui::CollapsingHeader("Multiplayer")) {
                static char serverIp[32] = "127.0.0.1";
                static char nickname[32] = "Player";
                ImGui::Text("Server IP: ");
                ImGui::SameLine();
                ImGui::InputText("##server_ip", serverIp, 32);
                ImGui::Text("Nickname: ");
                ImGui::SameLine();
                ImGui::InputText("##nickname", nickname, 32);
                if (ImGui::Button("Connect lol")) {}
            }

            ImGui::End();
        });
        return shouldExit;
    }
} // namespace MafiaMP::Core::States
