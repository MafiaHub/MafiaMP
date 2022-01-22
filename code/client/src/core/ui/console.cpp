#include "console.h"
#include "../application.h"
#include <utils/safe_win32.h>
#include <logging/logger.h>
#include <spdlog/spdlog.h>
#include <regex>
#include <imgui/imgui.h>

#include "../../sdk/entities/c_car.h"
#include "../../sdk/entities/c_player_2.h"
#include "../../sdk/entities/c_vehicle.h"
#include "../../game/helpers/camera.h"
#include "../../game/helpers/controls.h"
#include "../../sdk/mafia/framework/c_mafia_framework.h"
#include "../../sdk/mafia/framework/c_mafia_framework_interfaces.h"

#include "../application.h"

namespace MafiaMP::Core::UI {
    void Console::Toggle() {
        if (_isOpen)
            Close();
        else
            Open();
    }

    bool Console::Close() {
        _isOpen = false;

        // Lock game controls
        Game::Helpers::Controls::Lock(false);

        // Hide cursor
        gApplication->GetImGUI()->ShowCursor(false);
        return true;
    }

    bool Console::Update() {
        if (!_isOpen) {
            return _isOpen;
        }

        if (GetAsyncKeyState(VK_F1) & 0x1) {
            SpawnCar();
        }

        if (GetAsyncKeyState(VK_F3) & 0x1) {
            DespawnAll();
        }

        if (GetAsyncKeyState(VK_F5) & 0x1) {
            Disconnect();
        }

        gApplication->GetImGUI()->PushWidget([this]() {
            auto ringBuffer = Framework::Logging::GetInstance()->GetRingBuffer();

            if (!ImGui::Begin("Console", &_shouldDisplayWidget, ImGuiWindowFlags_MenuBar)) {
                ImGui::End();
                return;
            }

            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("Tools")) {
                    if (ImGui::MenuItem("Spawn car", "F1")) {
                        SpawnCar();
                    }
                    if (ImGui::MenuItem("Despawn all", "F3")) {
                        DespawnAll();
                    }
                    if (ImGui::MenuItem("Disconnect", "F5")) {
                        Disconnect();
                    }
                    if (ImGui::MenuItem("Close", "F8")) {
                        Close();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Log Level")) {
                    if (ImGui::MenuItem("Trace")) {
                        ringBuffer->set_level(spdlog::level::trace);
                    }
                    if (ImGui::MenuItem("Debug")) {
                        ringBuffer->set_level(spdlog::level::debug);
                    }
                    if (ImGui::MenuItem("Info")) {
                        ringBuffer->set_level(spdlog::level::info);
                    }
                    if (ImGui::MenuItem("Warn")) {
                        ringBuffer->set_level(spdlog::level::warn);
                    }
                    if (ImGui::MenuItem("Error")) {
                        ringBuffer->set_level(spdlog::level::err);
                    }
                    if (ImGui::MenuItem("Critical")) {
                        ringBuffer->set_level(spdlog::level::critical);
                    }
                    if (ImGui::MenuItem("Off")) {
                        ringBuffer->set_level(spdlog::level::off);
                    }
                    if (ImGui::MenuItem("N")) {
                        ringBuffer->set_level(spdlog::level::n_levels);
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Output");
            ImGui::Checkbox("Auto-scroll", &_autoScroll);
            ImGui::Separator();

            //TODO add color and styles
            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
            if (ringBuffer != nullptr) {
                std::vector<std::string> log_message = ringBuffer -> last_formatted();
                for (std::string &log : log_message) {
                    FormatLog(log);
                }
            }
            if (_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(0.0f);
            ImGui::EndChild();

            ImGui::Separator();

            static char consoleText[32] = "";
            ImGui::InputText("##console_text", consoleText, 32);

            ImGui::End();
        });

        return true;
    }

    bool Console::Open() {
        // Lock game controls
        Game::Helpers::Controls::Lock(true);

        // Enable cursor
        gApplication->GetImGUI()->ShowCursor(true);

        _isOpen = true;

        return true;
    }

    void Console::Disconnect() {
        Core::gApplication->GetNetworkingEngine()->GetNetworkClient()->Disconnect();
    }

    void Console::DespawnAll() {
        for (const auto &vehicle : _TEMP_vehicles) {
            Core::gApplication->GetEntityFactory()->ReturnEntity(vehicle);
        }
        _TEMP_vehicles.clear();
    }

    void Console::SpawnCar() {
        auto info = Core::gApplication->GetEntityFactory()->RequestVehicle("berkley_810");
        _TEMP_vehicles.push_back(info);

        const auto OnCarRequestFinish = [&](Game::Streaming::EntityTrackingInfo *info, bool success) {
            if (success) {
                auto car = reinterpret_cast<SDK::C_Car *>(info->GetEntity());
                if (!car) {
                    return;
                }
                car->GameInit();
                car->Activate();
                car->Unlock();

                auto localPlayer = SDK::GetGame()->GetActivePlayer();

                SDK::ue::sys::math::C_Vector newPos    = localPlayer->GetPos();
                SDK::ue::sys::math::C_Quat newRot      = localPlayer->GetRot();
                SDK::ue::sys::math::C_Matrix transform = {};
                transform.Identity();
                transform.SetRot(newRot);
                transform.SetPos(newPos);
                car->GetVehicle()->SetVehicleMatrix(transform, SDK::ue::sys::core::E_TransformChangeType::DEFAULT);
            }
        };

        const auto OnCarReturned = [&](Game::Streaming::EntityTrackingInfo *info, bool wasCreated) {
            if (!info) {
                return;
            }
            auto car = reinterpret_cast<SDK::C_Car *>(info->GetEntity());
            if (wasCreated && car) {
                car->Deactivate();
                car->GameDone();
                car->Release();
            }
        };

        info->SetRequestFinishCallback(OnCarRequestFinish);
        info->SetReturnCallback(OnCarReturned);
    }

    void Console::FormatLog(std::string log) {
        std::regex brackets_regex("\\[.*?\\]", std::regex_constants::ECMAScript);

        auto brackets_begin = std::sregex_iterator(log.begin(), log.end(), brackets_regex);
        auto brackets_end   = std::sregex_iterator();

        int logCount = 1;
        for (std::sregex_iterator i = brackets_begin; i != brackets_end; ++i) {
            std::smatch match     = *i;
            std::string match_str = match.str();

            if (logCount == 1) {
                ImGui::TextColored(ImVec4(0.5f, 1, 1, 1), "%s", match_str.c_str());
                ImGui::SameLine();
            }
            if (logCount == 2) {
                ImGui::TextColored(ImVec4(0.5f, 0.5f, 1, 1), "%s", match_str.c_str());
                ImGui::SameLine();
            }
            if (logCount == 3) {
                if (match_str.compare("[info]") == 0)
                    ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", match_str.c_str());
                else if (match_str.compare("[debug]") == 0)
                    ImGui::TextColored(ImVec4(0, 0, 1, 1), "%s", match_str.c_str());
                else if (match_str.compare("[error]") == 0)
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", match_str.c_str());
                else if (match_str.compare("[warning]") == 0)
                    ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", match_str.c_str());
                else if (match_str.compare("[trace]") == 0)
                    ImGui::TextColored(ImVec4(0.5f, 0, 1, 1), "%s", match_str.c_str());
                else if (match_str.compare("[critical]") == 0)
                    ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "%s", match_str.c_str());
                else
                    ImGui::TextColored(ImVec4(1, 0, 0.5f, 1), "%s", match_str.c_str());
                
                ImGui::SameLine();
                const auto &suffix = match.suffix();
                if (suffix.length() > 0)
                    ImGui::Text(suffix.str().c_str());
            }
            logCount++;
        }
    }
} // namespace MafiaMP::Core::Modules
