#include "console.h"
#include "../application.h"
#include <utils/safe_win32.h>
#include <logging/logger.h>
#include <spdlog/spdlog.h>
#include <regex>
#include <sstream>
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
    Console::Console(std::shared_ptr<Framework::Utils::States::Machine> machine): _machine(machine) {
        RegisterCommand("crash", [this] (const std::string &, const std::vector<std::string> &) {
            CrashMe();
        });
        RegisterCommand("echo", [this] (const std::string &input, const std::vector<std::string> &args) {
            std::string argsConcat;
            for (auto &arg : args) {
                argsConcat += arg + " ";
            }
            Framework::Logging::GetLogger("Console")->info("echo input: '{}', args: {}", input, argsConcat);
        });
    }

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
                    if (ImGui::MenuItem("Crash me!")) {
                        CrashMe();
                    }
                    if (ImGui::MenuItem("Break me!")) {
                        BreakMe();
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

            static char consoleText[512] = "";
            if (ImGui::InputText("##console_text", consoleText, 512, ImGuiInputTextFlags_EnterReturnsTrue|ImGuiInputTextFlags_CtrlEnterForNewLine)) {
                ProcessCommand(consoleText);
                consoleText[0] = '\0';
                ImGui::SetKeyboardFocusHere(-1);
            }
            if (_focusOnConsole) {
                ImGui::SetKeyboardFocusHere(-1);
                _focusOnConsole = false;
            }
            ImGui::SameLine();

            // show autocomplete
            static bool isAutocompleteOpen = false;
            std::vector<std::string> allCommands, filteredCommands;
            for (auto &command : _commands) {
                allCommands.push_back(command.first);
            }
            bool isFocused = ImGui::IsItemFocused();
            isAutocompleteOpen |= ImGui::IsItemActive();

            // todo replace with std code, C used down here! 
            for (auto &command : allCommands) {
                if (strnicmp(command.c_str(), consoleText, strlen(consoleText)) == 0) {
                    filteredCommands.push_back(command);
                }
            }

            if (isAutocompleteOpen && filteredCommands.size() > 0 && strlen(consoleText) > 0) {
                ImGui::SetNextWindowPos({ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y});
                ImGui::SetNextWindowSize({ImGui::GetItemRectSize().x, 0});
                if (ImGui::Begin("##popup", &isAutocompleteOpen, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_Tooltip)) {
                    isFocused |= ImGui::IsWindowFocused();
                    for (auto &command : filteredCommands) {
                        if (strstr(command.c_str(), consoleText) == NULL)
                            continue;
                        if (ImGui::Selectable(command.c_str()) || (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))) {
                            strcpy(consoleText, command.c_str());
                            isAutocompleteOpen = false;
                        }
                    }
                }
                ImGui::End();
                isAutocompleteOpen &= isFocused;
            }

            ImGui::SameLine();
            if (ImGui::Button("Send")) {
                ProcessCommand(consoleText);
                consoleText[0] = '\0';
                _focusOnConsole = true;
            }

            ImGui::PushAllowKeyboardFocus( false );

            // Draw arbitrary popup content...

            ImGui::PopAllowKeyboardFocus();

            ImGui::End();
        });

        return true;
    }

    /* todo move to utils */
    std::string ltrim(const std::string &s) {
        return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
    }

    std::string rtrim(const std::string &s) {
        return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
    }

    std::string trim(const std::string &s) {
        return ltrim(rtrim(s));
    }

    void Console::ProcessCommand(const std::string &input) {
        std::string command = "";
        std::string argsPart = "";
        std::vector<std::string> args;
        std::stringstream parts(input);
        std::string item;
        int i = 0;
        while (std::getline(parts, item, ' ')) {
            if (i > 0) {
                argsPart += item + " ";
            }
            args.push_back(item);
            ++i;
        }

        if (args.size() == 0) {
            return;
        }

        command = args[0];

        if (_commands.find(command) != _commands.end()) {
            _commands[command](trim(argsPart), std::vector<std::string>(args.begin() + 1, args.end()));
        } else {
            Framework::Logging::GetLogger("Console")->warn("Unknown command: {}", input);
        }
    }

    bool Console::Open() {
        // Lock game controls
        Game::Helpers::Controls::Lock(true);

        // Enable cursor
        gApplication->GetImGUI()->ShowCursor(true);

        _isOpen = true;
        _focusOnConsole = true;

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

    void Console::CrashMe() {
        *(int*)5 = 5;
    }

    void Console::BreakMe() {
        __debugbreak();
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
                    ImGui::Text("%s", suffix.str().c_str());
            }
            logCount++;
        }
    }

    void Console::RegisterCommand(const std::string &name, const CommandProc &proc) {
        if (name.empty()) {
            return;
        }

        _commands[name] = proc;
    }
} // namespace MafiaMP::Core::Modules
