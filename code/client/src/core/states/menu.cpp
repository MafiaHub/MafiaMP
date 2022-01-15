#include "menu.h"
#include "states.h"

#include <utils/states/machine.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx11.h>

#include "../application.h"

#include "../../sdk/mafia/framework/c_mafia_framework_interfaces.h"
#include "../../sdk/mafia/framework/c_mafia_framework.h"

#include "../../sdk/c_game_input_module.h"
#include "../../sdk/entities/c_player_2.h"

namespace MafiaMP::Core::States {
    InMenuState::InMenuState() {}

    InMenuState::~InMenuState() {}

    int32_t InMenuState::GetId() const {
        return StateIds::Menu;
    }

    const char *InMenuState::GetName() const {
        return "InMenu";
    }

    void InMenuState::LockPlayerControls(bool lock) {
        SDK::mafia::framework::C_MafiaFramework *pMafiaFramework                     = SDK::mafia::framework::C_MafiaFramework::GetInstance();
        if (!pMafiaFramework) {
            return;
        }

        SDK::mafia::framework::C_MafiaFrameworkInterfaces *pMafiaFrameworkInterfaces = pMafiaFramework->GetInterfaces();
        if (!pMafiaFrameworkInterfaces) {
            return;
        }
        SDK::C_Game *pGame                                                  = pMafiaFrameworkInterfaces->GetGame();
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

    bool InMenuState::OnEnter(Framework::Utils::States::Machine *) {
        // Set camera
        //TODO

        // Lock game controls
        LockPlayerControls(true);

        // Enable cursor
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDrawCursor = true;
        return true;
    }

    bool InMenuState::OnExit(Framework::Utils::States::Machine *) {
        // Hide cursor
        ImGuiIO &io        = ImGui::GetIO();
        io.MouseDrawCursor = false;

        // Unlock game controls
        LockPlayerControls(false);

        // Reset camera by player
        //TODO
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
