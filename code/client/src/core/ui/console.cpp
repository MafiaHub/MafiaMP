#include "console.h"
#include "../application.h"
#include <utils/safe_win32.h>
#include <logging/logger.h>
#include <spdlog/spdlog.h>

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
    Console::Console(Framework::Utils::States::Machine *m) {
        _machine = m;
        Init();
    }

    Console::~Console() {}

    bool Console::Init() {
        _shouldDisplayWidget = true;
        _autoScroll          = true;
        return true;
    }

    void Console::Toggle() {
        if (_isOpen)
            Close();
        else
            Open();
    }

    bool Console::Close() {
        _isOpen = false;
        // Temp
        Game::Helpers::Camera::ResetBehindPlayer();

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
                for (auto &log : log_message) {
                    ImGui::Text("%s", log.c_str());
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
        // Set camera
        Game::Helpers::Camera::SetPos({450.43698, -646.01941, 58.132675}, {-399.2962, -594.75391, 37.324718}, true);

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
        for (size_t i = 0; i < _TEMP_vehicles.size(); i++) { 
            Core::gApplication->GetEntityFactory()->ReturnEntity(_TEMP_vehicles[i]); 
        }
        _TEMP_vehicles.clear();
    }

    void Console::SpawnCar() {
        printf("asking car\n");
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
} // namespace MafiaMP::Core::Modules
