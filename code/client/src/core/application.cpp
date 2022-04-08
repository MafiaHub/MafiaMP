#include "application.h"

#include <logging/logger.h>

#include "states/initialize.h"
#include "states/menu.h"
#include "states/session_connected.h"
#include "states/session_connection.h"
#include "states/session_disconnection.h"
#include "states/session_offline_debug.h"
#include "states/shutdown.h"
#include "states/states.h"

#include "../shared/messages/human/human_despawn.h"
#include "../shared/messages/human/human_self_update.h"
#include "../shared/messages/human/human_spawn.h"
#include "../shared/messages/human/human_update.h"

#include "../game/helpers/camera.h"
#include "../game/helpers/controls.h"
#include "../game/streaming/entity_factory.h"
#include "../sdk/entities/c_car.h"
#include "../sdk/entities/c_player_2.h"
#include "../sdk/entities/c_vehicle.h"
#include "../sdk/mafia/framework/c_mafia_framework.h"
#include "../sdk/mafia/framework/c_mafia_framework_interfaces.h"

#include "../shared/modules/human_sync.hpp"

#include "external/imgui/widgets/corner_text.h"

#include "../shared/version.h"
#include <utils/version.h>
#include <cppfs/FileHandle.h>
#include <cppfs/fs.h>

#include "modules/human.hpp"

namespace MafiaMP::Core {
    std::unique_ptr<Application> gApplication = nullptr;

    bool Application::PostInit() {
        // Create the state machine and initialize
        _stateMachine = std::make_shared<Framework::Utils::States::Machine>();
        _stateMachine->RegisterState<States::InitializeState>();
        _stateMachine->RegisterState<States::InMenuState>();
        _stateMachine->RegisterState<States::SessionConnectionState>();
        _stateMachine->RegisterState<States::SessionConnectedState>();
        _stateMachine->RegisterState<States::SessionDisconnectionState>();
        _stateMachine->RegisterState<States::SessionOfflineDebugState>();
        _stateMachine->RegisterState<States::ShutdownState>();

        // Create the entity factory
        _entityFactory = std::make_shared<Game::Streaming::EntityFactory>();

        // Register other things
        InitNetworkingMessages();

        // This must always be the last call
        _stateMachine->RequestNextState(States::StateIds::Initialize);

        _commandProcessor = std::make_shared<Framework::Utils::CommandProcessor>();
        _console          = std::make_shared<UI::MafiaConsole>(_commandProcessor);

        // setup debug routines
        SetupCommands();
        SetupMenuBar();

        // Register client modules
        GetWorldEngine()->GetWorld()->import<Modules::Human>();
        GetWorldEngine()->GetWorld()->import<Shared::Modules::HumanSync>();

        GetWorldEngine()->SetOnEntityDestroyCallback([](flecs::entity e) {
            if (e.get<Core::Modules::Human::Tracking>() != nullptr) {
                Core::Modules::Human::RemoveHuman(e);
            }

            return true;
        });

        // Setup Lua VM wrapper
        _luaVM = std::make_shared<LuaVM>();

        return true;
    }

    bool Application::PreShutdown() {
        if (_entityFactory) {
            _entityFactory->ReturnAll();
        }
        return true;
    }

    void Application::PostUpdate() {
        if (_stateMachine) {
            _stateMachine->Update();
        }

        if (_entityFactory) {
            _entityFactory->Update();
        }

        static bool isStyleInitialized = false;
        if (!isStyleInitialized) {
            PimpMyImGUI();
            isStyleInitialized = true;
        }

        // Tick discord instance - Temporary
        const auto discordApi = Core::gApplication->GetPresence();
        if (discordApi && discordApi->IsInitialized()) {
            discordApi->SetPresence("Freeroam", "Screwing around", discord::ActivityType::Playing);
        }

#if 1
        Core::gApplication->GetImGUI()->PushWidget([&]() {
            using namespace Framework::External::ImGUI::Widgets;
            const auto networkClient = Core::gApplication->GetNetworkingEngine()->GetNetworkClient();
            const auto connState     = networkClient->GetConnectionState();
            const auto ping          = networkClient->GetPing();

            _console->Update();

            if (GetAsyncKeyState(VK_F8) & 0x1) {
                _console->Toggle();
            }

            const char *connStateNames[] = {"Connecting", "Online", "Offline"};

            // versioning
            DrawCornerText(CORNER_RIGHT_BOTTOM, "Mafia: Multiplayer");
            DrawCornerText(CORNER_RIGHT_BOTTOM, fmt::format("Framework version: {} ({})", Framework::Utils::Version::rel, Framework::Utils::Version::git));
            DrawCornerText(CORNER_RIGHT_BOTTOM, fmt::format("MafiaMP version: {} ({})", MafiaMP::Version::rel, MafiaMP::Version::git));

            // connection details
            DrawCornerText(CORNER_LEFT_BOTTOM, fmt::format("Connection: {}", connStateNames[connState]));
            DrawCornerText(CORNER_LEFT_BOTTOM, fmt::format("Ping: {}", ping));
        });

        if (_console->IsOpen()) {
            if (GetAsyncKeyState(VK_F1) & 0x1) {
                SpawnCar();
            }

            if (GetAsyncKeyState(VK_F3) & 0x1) {
                DespawnAll();
            }

            if (GetAsyncKeyState(VK_F5) & 0x1) {
                Disconnect();
            }
        }
#endif
    }

    void Application::InitNetworkingMessages() {
        SetOnConnectionFinalizedCallback([this](flecs::entity newPlayer, float tickInterval) {
            _tickInterval = tickInterval;
            _stateMachine->RequestNextState(States::StateIds::SessionConnected);
            auto localPlayer = Game::Helpers::Controls::GetLocalPlayer();

            _localPlayer = newPlayer;

            auto trackingData   = _localPlayer.get_mut<Core::Modules::Human::Tracking>();
            trackingData->human = localPlayer;
            trackingData->info  = nullptr;

            _localPlayer.add<Shared::Modules::HumanSync::UpdateData>();
            _localPlayer.add<Core::Modules::Human::LocalPlayer>();

            const auto es            = _localPlayer.get_mut<Framework::World::Modules::Base::Streamable>();
            es->modEvents.updateProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                const auto updateData = e.get<Shared::Modules::HumanSync::UpdateData>();

                Shared::Messages::Human::HumanUpdate humanUpdate;
                humanUpdate.FromParameters(GetWorldEngine()->GetServerID(e));
                humanUpdate.SetCharStateHandlerType(updateData->_charStateHandlerType);
                humanUpdate.SetHealthPercent(updateData->_healthPercent);
                humanUpdate.SetMoveMode(updateData->_moveMode);
                humanUpdate.SetSprinting(updateData->_isSprinting);
                humanUpdate.SetSprintSpeed(updateData->_sprintSpeed);
                humanUpdate.SetStalking(updateData->_isStalking);
                peer->Send(humanUpdate, guid);
                return true;
            };
        });

        SetOnConnectionClosedCallback([this]() {
            _stateMachine->RequestNextState(States::StateIds::SessionDisconnection);
        });

        // TEMP hook up human events
        const auto net = GetNetworkingEngine()->GetNetworkClient();

        net->RegisterMessage<Shared::Messages::Human::HumanSpawn>(Shared::Messages::ModMessages::MOD_HUMAN_SPAWN, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanSpawn *msg) {
            auto e = GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            // Setup tracking info
            Core::Modules::Human::CreateHuman(e, msg->GetSpawnProfile());

            // Setup other components
            e.add<Shared::Modules::HumanSync::UpdateData>();

            // set up client updates (NPC streaming)
            // TODO disabled for now, we don't really need to stream NPCs atm
#if 0
            const auto es = e.get_mut<Framework::World::Modules::Base::Streamable>();
            es->modEvents.clientUpdateProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                Shared::Messages::Human::HumanClientUpdate humanUpdate;
                humanUpdate.FromParameters(e.id());
                // set up sync data
                peer->Send(humanUpdate, guid);
                return true;
            };
#endif
        });
        net->RegisterMessage<Shared::Messages::Human::HumanDespawn>(Shared::Messages::ModMessages::MOD_HUMAN_DESPAWN, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanDespawn *msg) {
            const auto e = GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            Core::Modules::Human::RemoveHuman(e);
        });
        net->RegisterMessage<Shared::Messages::Human::HumanUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_UPDATE, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanUpdate *msg) {
            const auto e = GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            auto updateData                   = e.get_mut<Shared::Modules::HumanSync::UpdateData>();
            updateData->_charStateHandlerType = msg->GetCharStateHandlerType();
            updateData->_healthPercent        = msg->GetHealthPercent();
            updateData->_isSprinting          = msg->IsSprinting();
            updateData->_isStalking           = msg->IsStalking();
            updateData->_moveMode             = msg->GetMoveMode();
            updateData->_sprintSpeed          = msg->GetSprintSpeed();

            Core::Modules::Human::UpdateHuman(e);
        });
        net->RegisterMessage<Shared::Messages::Human::HumanSelfUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_SELF_UPDATE, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanSelfUpdate *msg) {
            const auto e = GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            const auto trackingData = e.get<Core::Modules::Human::Tracking>();
            if (!trackingData) {
                return;
            }

            // update actor data
        });
    }

    void Application::PimpMyImGUI() {
        ImVec4 *colors                         = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.72f, 0.00f, 0.00f, 0.54f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.90f, 0.00f, 0.00f, 0.40f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.83f, 0.00f, 0.00f, 0.67f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.41f, 0.03f, 0.03f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg]             = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.22f, 0.00f, 0.01f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.55f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.78f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_CheckMark]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrab]            = ImVec4(0.88f, 0.24f, 0.24f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
        colors[ImGuiCol_Button]                = ImVec4(0.70f, 0.00f, 0.00f, 0.40f);
        colors[ImGuiCol_ButtonHovered]         = ImVec4(0.61f, 0.01f, 0.09f, 1.00f);
        colors[ImGuiCol_ButtonActive]          = ImVec4(0.78f, 0.02f, 0.02f, 1.00f);
        colors[ImGuiCol_Header]                = ImVec4(0.75f, 0.00f, 0.00f, 0.31f);
        colors[ImGuiCol_HeaderHovered]         = ImVec4(0.78f, 0.00f, 0.00f, 0.80f);
        colors[ImGuiCol_HeaderActive]          = ImVec4(0.86f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_Separator]             = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive]       = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGrip]            = ImVec4(0.98f, 0.26f, 0.26f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
        colors[ImGuiCol_Tab]                   = ImVec4(0.42f, 0.00f, 0.00f, 0.86f);
        colors[ImGuiCol_TabHovered]            = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
        colors[ImGuiCol_TabActive]             = ImVec4(0.67f, 0.01f, 0.01f, 1.00f);
        colors[ImGuiCol_TabUnfocused]          = ImVec4(0.23f, 0.00f, 0.00f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.36f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight]      = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.55f, 0.02f, 0.02f, 0.35f);
        colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]          = ImVec4(0.62f, 0.01f, 0.01f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

        ImGui::GetStyle().FrameRounding            = 0.5f;
        ImGui::GetStyle().WindowRounding           = 0.5f;
        ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_Right;
        ImGui::GetStyle().WindowTitleAlign         = {0.5f, 0.5f};
    }

    void Application::Disconnect() {
        GetNetworkingEngine()->GetNetworkClient()->Disconnect();
    }

    void Application::DespawnAll() {
        for (const auto &vehicle : _TEMP_vehicles) { GetEntityFactory()->ReturnEntity(vehicle); }
        _TEMP_vehicles.clear();
    }

    void Application::CrashMe() {
        *(int *)5 = 5;
    }

    void Application::BreakMe() {
        __debugbreak();
    }

    void Application::CloseGame() {
        // very lazy game shutdown
        // don't try at home
        ExitProcess(0);
    }

    void Application::SpawnCar(const std::string modelName) {
        auto info = Core::gApplication->GetEntityFactory()->RequestVehicle(modelName);
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

    void Application::SetupCommands() {
        _commandProcessor->RegisterCommand(
            "test", {{"a,aargument", "Test argument 1", cxxopts::value<std::string>()}, {"b,bargument", "Test argument 2", cxxopts::value<int>()}},
            [this](cxxopts::ParseResult result) {
                if (result.count("aargument")) {
                    std::string argument1 = result["aargument"].as<std::string>();
                    Framework::Logging::GetLogger("Debug")->info("aargument - {}", argument1);
                }
                if (result.count("bargument")) {
                    int argument2 = result["bargument"].as<int>();
                    Framework::Logging::GetLogger("Debug")->info("bargument - {}", argument2);
                }
            },
            "Testing command");
        _commandProcessor->RegisterCommand(
            "crash", {},
            [this](cxxopts::ParseResult &) {
                CrashMe();
            },
            "crashes the game");
        _commandProcessor->RegisterCommand(
            "echo", {},
            [this](cxxopts::ParseResult result) {
                std::string argsConcat;
                cxxopts::PositionalList args = result.unmatched();
                for (auto &arg : args) { argsConcat += arg + " "; }
                Framework::Logging::GetLogger("Debug")->info(argsConcat);
            },
            "[args] - prints the arguments back");
        _commandProcessor->RegisterCommand(
            "help", {},
            [this](cxxopts::ParseResult &) {
                std::stringstream ss;
                for (const auto &name : _commandProcessor->GetCommandNames()) { ss << fmt::format("{} {:>8}\n", name, _commandProcessor->GetCommandInfo(name)->options->help()); }
                Framework::Logging::GetLogger("Debug")->info("Available commands:\n{}", ss.str());
            },
            "prints all available commands");
        _commandProcessor->RegisterCommand(
            "exit", {},
            [this](cxxopts::ParseResult &) {
                CloseGame();
            },
            "quits the game");
        _commandProcessor->RegisterCommand(
            "spawnCar", {{"m,model", "model name of the car", cxxopts::value<std::string>()->default_value("berkley_810")}},
            [this](cxxopts::ParseResult result) {
                std::string modelName = result["model"].as<std::string>();
                SpawnCar(modelName);
            },
            "spawn a car of a given model");
        _commandProcessor->RegisterCommand(
            "lua", {
                {"c,command", "command to execute", cxxopts::value<std::string>()->default_value("")},
                {"f,file", "file to execute", cxxopts::value<std::string>()->default_value("")}
            },
            [this](cxxopts::ParseResult result) {
                std::string command = result["command"].as<std::string>();
                if (!command.empty()) {
                    _luaVM->ExecuteString(command.c_str());
                }
                std::string filename = result["file"].as<std::string>();
                if (!filename.empty()) {
                    // todo use prefix path for lua scripts? (currently hardcoded to "scripts/")
                    cppfs::FileHandle file = cppfs::fs::open(gProjectPath + "/scripts/" + filename);
                    if (!file.exists()) {
                        Framework::Logging::GetLogger(LOG_LUA)->warn("Script does not exist");
                        return;
                    }
                    if (!file.isFile()) {
                        Framework::Logging::GetLogger(LOG_LUA)->warn("Script is not a file");
                        return;
                    }
                    std::string content = file.readFile();
                    if (content.empty()) {
                        Framework::Logging::GetLogger(LOG_LUA)->warn("Script is empty");
                        return;
                    }
                    _luaVM->ExecuteString(content.c_str());
                }
            },
            "executes Lua commands");
    }

    void Application::SetupMenuBar() {
        _console->RegisterMenuBarDrawer([this]() {
            if (ImGui::BeginMenu("Debug")) {
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
                if (ImGui::MenuItem("Exit Game")) {
                    CloseGame();
                }
                ImGui::EndMenu();
            }
        });
    }

    std::string gProjectPath;
} // namespace MafiaMP::Core
