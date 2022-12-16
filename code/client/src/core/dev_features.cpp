/*
 * MafiaHub OSS license
 * Copyright (c) 2022, MafiaHub. All rights reserved.
 *
 * This file comes from MafiaHub, hosted at https://github.com/MafiaHub/Framework.
 * See LICENSE file in the source repository for information regarding licensing.
 */

#include "dev_features.h"
#include "application.h"

#include <logging/logger.h>

#include "game/helpers/controls.h"
#include "game/helpers/human.h"
#include "game/streaming/entity_factory.h"
#include "sdk/entities/c_car.h"
#include "sdk/entities/c_player_2.h"
#include "sdk/entities/c_vehicle.h"
#include "sdk/mafia/framework/c_mafia_framework_interfaces.h"

#include "external/imgui/widgets/corner_text.h"

#include <cppfs/FileHandle.h>
#include <cppfs/fs.h>

#include "shared/rpc/chat_message.h"
#include "shared/rpc/spawn_car.h"

#include "modules/human.h"

namespace MafiaMP::Core {
    DevFeatures::DevFeatures() {
        _entityBrowser = std::make_shared<UI::EntityBrowser>();
    }

    void DevFeatures::Init() {
        SetupCommands();
        SetupMenuBar();
    }

    void DevFeatures::Update() {
        if (_showEntityBrowser)
            _entityBrowser->Update();
    }

    void DevFeatures::Shutdown() {}

    void DevFeatures::Disconnect() {
        gApplication->GetNetworkingEngine()->GetNetworkClient()->Disconnect();
    }

    void DevFeatures::DespawnAll() {
        for (const auto &vehicle : _TEMP_vehicles) {
            gApplication->GetEntityFactory()->ReturnEntity(vehicle);
        }
        _TEMP_vehicles.clear();
    }

    void DevFeatures::CrashMe() {
        *(int *)5 = 5;
    }

    void DevFeatures::BreakMe() {
        __debugbreak();
    }

    void DevFeatures::CloseGame() {
        // very lazy game shutdown
        // don't try at home
        ExitProcess(0);
    }

    void DevFeatures::ToggleEntityBrowser() {
        _showEntityBrowser = !_showEntityBrowser;
    }

    void DevFeatures::SpawnCar(std::string modelName) {
        const auto net = gApplication->GetNetworkingEngine()->GetNetworkClient();
        if (net->GetConnectionState() == Framework::Networking::CONNECTED) {
            Shared::RPC::SpawnCar spawnCarMsg {};
            spawnCarMsg.SetModelName(modelName);
            net->SendRPC(spawnCarMsg);
        }
        else {
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
    }

    void DevFeatures::SetupCommands() {
        gApplication->_commandProcessor->RegisterCommand(
            "test", {{"a,aargument", "Test argument 1", cxxopts::value<std::string>()}, {"b,bargument", "Test argument 2", cxxopts::value<int>()}},
            [this](const cxxopts::ParseResult &result) {
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
        gApplication->_commandProcessor->RegisterCommand(
            "crash", {},
            [this](cxxopts::ParseResult &) {
                CrashMe();
            },
            "crashes the game");
        gApplication->_commandProcessor->RegisterCommand(
            "echo", {},
            [this](const cxxopts::ParseResult &result) {
                std::string argsConcat;
                cxxopts::PositionalList args = result.unmatched();
                for (auto &arg : args) {
                    argsConcat += arg + " ";
                }
                Framework::Logging::GetLogger("Debug")->info(argsConcat);
            },
            "[args] - prints the arguments back");
        gApplication->_commandProcessor->RegisterCommand(
            "help", {},
            [this](cxxopts::ParseResult &) {
                std::stringstream ss;
                for (const auto &name : gApplication->_commandProcessor->GetCommandNames()) {
                    ss << fmt::format("{} {:>8}\n", name, gApplication->_commandProcessor->GetCommandInfo(name)->options->help());
                }
                Framework::Logging::GetLogger("Debug")->info("Available commands:\n{}", ss.str());
            },
            "prints all available commands");
        gApplication->_commandProcessor->RegisterCommand(
            "exit", {},
            [this](cxxopts::ParseResult &) {
                CloseGame();
            },
            "quits the game");
        gApplication->_commandProcessor->RegisterCommand(
            "spawnCar", {{"m,model", "model name of the car", cxxopts::value<std::string>()->default_value("berkley_810")}},
            [this](const cxxopts::ParseResult &result) {
                std::string modelName = result["model"].as<std::string>();
                SpawnCar(modelName);
            },
            "spawn a car of a given model");
        gApplication->_commandProcessor->RegisterCommand(
            "lua", {{"c,command", "command to execute", cxxopts::value<std::string>()->default_value("")}, {"f,file", "file to execute", cxxopts::value<std::string>()->default_value("")}},
            [this](const cxxopts::ParseResult &result) {
                std::string command = result["command"].as<std::string>();
                if (!command.empty()) {
                    gApplication->_luaVM->ExecuteString(command.c_str());
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
                    gApplication->_luaVM->ExecuteString(content.c_str());
                }
            },
            "executes Lua commands");
        gApplication->_commandProcessor->RegisterCommand(
            "chat", {{"m,msg", "message to send", cxxopts::value<std::string>()->default_value("")}},
            [this](const cxxopts::ParseResult &result) {
                const auto net = gApplication->GetNetworkingEngine()->GetNetworkClient();
                if (net->GetConnectionState() == Framework::Networking::CONNECTED) {
                    MafiaMP::Shared::RPC::ChatMessage chatMessage {};
                    chatMessage.FromParameters(result["msg"].as<std::string>());
                    net->SendRPC(chatMessage, SLNet::UNASSIGNED_RAKNET_GUID);
                }
            },
            "sends a chat message");
        gApplication->_commandProcessor->RegisterCommand(
            "disconnect", {},
            [this](const cxxopts::ParseResult &) {
                Disconnect();
            },
            "disconnect from server");
        gApplication->_commandProcessor->RegisterCommand(
            "wep", {{"w,wep", "weapon id", cxxopts::value<int>()->default_value("85")}, {"a,ammo", "ammo count", cxxopts::value<int>()->default_value("200")}},
            [this](const cxxopts::ParseResult &result) {
                const auto human = Game::Helpers::Controls::GetLocalPlayer();
                if (human) {
                    auto wep  = result["wep"].as<int>();
                    auto ammo = result["ammo"].as<int>();
                    Game::Helpers::Human::AddWeapon(human, wep, ammo);
                    Framework::Logging::GetLogger("test")->debug("Added wep {} with ammo {}", wep, ammo);
                }
            },
            "sends a chat message");
        gApplication->_commandProcessor->RegisterCommand(
            "showEntityBrowser", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleEntityBrowser();
            },
            "toggles entity browser dialog");
    }

    void DevFeatures::SetupMenuBar() {
        gApplication->_console->RegisterMenuBarDrawer([this]() {
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
            if (ImGui::BeginMenu("Editors")) {
                if (ImGui::MenuItem("Entity Browser")) {
                    ToggleEntityBrowser();
                }
                ImGui::EndMenu();
            }
        });
    }

} // namespace MafiaMP::Core
