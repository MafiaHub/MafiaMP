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
#include "game/overrides/character_controller.h"
#include "game/streaming/entity_factory.h"

#include "sdk/entities/c_crash_obj.h"
#include "sdk/entities/c_player_2.h"
#include "sdk/mafia/framework/c_mafia_framework_interfaces.h"

#include <imgui.h>

#include <cppfs/FileHandle.h>
#include <cppfs/fs.h>

#include "shared/rpc/chat_message.h"

namespace MafiaMP::Core {
    DevFeatures::DevFeatures() {
        _audioDebug    = std::make_shared<UI::AudioDebug>();
        _cameraStudio  = std::make_shared<UI::CameraStudio>();
        _entityBrowser = std::make_shared<UI::EntityBrowser>();
        _networkStats  = std::make_shared<UI::NetworkStats>();
        _playerDebug   = std::make_shared<UI::PlayerDebug>();
        _vehicleDebug  = std::make_shared<UI::VehicleDebug>();
        _worldDebug    = std::make_shared<UI::WorldDebug>();
    }

    void DevFeatures::Init() {
        SetupCommands();
        SetupMenuBar();
    }

    void DevFeatures::Update() {
        if (_audioDebug->IsVisible()) {
            _audioDebug->Update();
        }

        if (_entityBrowser->IsVisible()) {
            _entityBrowser->Update();
        }

        if (_cameraStudio->IsVisible()) {
            _cameraStudio->Update();
        }

        if (_playerDebug->IsVisible()) {
            _playerDebug->Update();
        }

        if (_vehicleDebug->IsVisible()) {
            _vehicleDebug->Update();
        }

        if (_networkStats->IsVisible()) {
            _networkStats->Update();
        }

        if (_worldDebug->IsVisible()) {
            _worldDebug->Update();
        }

        /**
         * F8 is for console
         * F9 is for main menu
         * F12 is for Steam screenshot
         */

        if (gApplication->_input->IsKeyPressed(FW_KEY_F1)) {
            ToggleWorldDebug();
        }

        if (gApplication->_input->IsKeyPressed(FW_KEY_F2)) {
            TogglePlayerDebug();
        }

        if (gApplication->_input->IsKeyPressed(FW_KEY_F3)) {
            ToggleVehicleDebug();
        }

        static bool bTempLockControls = false;
        if (gApplication->_input->IsKeyPressed(FW_KEY_F4)) {
            //gApplication->GetImGUI()->ShowCursor(!_cameraStudio->IsVisible());
            //MafiaMP::Game::Helpers::Controls::Lock(!_cameraStudio->IsVisible());
            //ToggleCameraStudio();
            bTempLockControls = !bTempLockControls;
            gApplication->LockControls(bTempLockControls);
        }

        if (gApplication->_input->IsKeyPressed(FW_KEY_F5)) {
            // If human is already created, delete it first
            if (_TEMP_HUMAN) {
                gApplication->GetEntityFactory()->ReturnEntity(_TEMP_HUMAN);
                _TEMP_HUMAN = nullptr;
            }

            // Otherwise always proceed with creation
            _TEMP_HUMAN = Core::gApplication->GetEntityFactory()->RequestHuman(10029431515544697714);

            const auto OnHumanRequestFinish = [](Game::Streaming::EntityTrackingInfo *info, bool success) {
                CreateNetCharacterController = false;
                if (success) {
                    auto human = reinterpret_cast<SDK::C_Human2 *>(info->GetEntity());
                    if (!human) {
                        return;
                    }
                    human->GameInit();
                    human->Activate();

                    const auto ent   = info->GetNetworkEntity();
                    auto localPlayer = SDK::GetGame()->GetActivePlayer();

                    SDK::ue::sys::math::C_Vector newPos    = localPlayer->GetPos();
                    SDK::ue::sys::math::C_Quat newRot      = localPlayer->GetRot();
                    SDK::ue::sys::math::C_Matrix transform = {};
                    transform.Identity();
                    transform.SetRot(newRot);
                    transform.SetPos(newPos);
                    human->SetTransform(transform);

                    // TODO(DavoSK): remove
                    Game::Helpers::Human::AddWeapon(human, 85, 200);
                    Game::Helpers::Human::AddWeapon(human, 3, 200);
                    Game::Helpers::Human::AddWeapon(human, 13, 200);
                    human->GetHumanWeaponController()->DoWeaponSelectByItemId(85, true);
                }
            };

            const auto OnHumanReturned = [](Game::Streaming::EntityTrackingInfo *info, bool wasCreated) {
                if (!info) {
                    return;
                }
                auto human = reinterpret_cast<SDK::C_Human2 *>(info->GetEntity());
                if (wasCreated && human) {
                    human->Deactivate();
                    human->GameDone();
                    human->Release();
                }
            };

            // setup tracking callbacks
            _TEMP_HUMAN->SetBeforeSpawnCallback([&](Game::Streaming::EntityTrackingInfo *) {
                CreateNetCharacterController = true;
            });
            _TEMP_HUMAN->SetRequestFinishCallback(OnHumanRequestFinish);
            _TEMP_HUMAN->SetReturnCallback(OnHumanReturned);
        }

        if (gApplication->_input->IsKeyPressed(FW_KEY_F6)) {
            if (!_TEMP_HUMAN) {
                return;
            }

            const auto human = reinterpret_cast<SDK::C_Human2 *>(_TEMP_HUMAN->GetEntity());

            auto localPlayer = SDK::GetGame()->GetActivePlayer();

            SDK::ue::sys::math::C_Vector newPos = localPlayer->GetPos();

            SDK::ue::C_CntPtr<uintptr_t> syncObject2;
            SDK::C_Entity *ent = reinterpret_cast<SDK::C_Entity *>(localPlayer);
            human->GetHumanScript()->ScrAim(syncObject2, !human->GetHumanWeaponController()->IsAiming());
            // human->GetHumanScript()->ScrAimAt(syncObject2, ent, newPos, !human->GetHumanWeaponController()->IsAiming());

            Framework::Logging::GetLogger("Playground")->debug("Aiming : {}", human->GetHumanWeaponController()->IsAiming());
        }

        if (gApplication->_input->IsKeyPressed(FW_KEY_F7)) {
            SpawnCrashObject();
        }

        if (gApplication->_input->IsKeyPressed(FW_KEY_F10)) {
            ToggleNetworkStats();
        }

        if (gApplication->_input->IsKeyPressed(FW_KEY_F11)) {
            ToggleEntityBrowser();
        }
    }

    void DevFeatures::Shutdown() {}

    void DevFeatures::SetupCommands() {
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
            "test", {},
            [this](cxxopts::ParseResult &) {

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
            "exit", {},
            [this](cxxopts::ParseResult &) {
                CloseGame();
            },
            "quits the game");

        gApplication->_commandProcessor->RegisterCommand(
            "disconnect", {},
            [this](const cxxopts::ParseResult &) {
                Disconnect();
            },
            "disconnect from server");

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
            "spawnCar", {{"m,model", "model name of the car", cxxopts::value<std::string>()->default_value("berkley_810")}},
            [this](const cxxopts::ParseResult &result) {
                std::string modelName = result["model"].as<std::string>();
                _worldDebug->SpawnCar(modelName);
            },
            "spawn a car of a given model");

        gApplication->_commandProcessor->RegisterCommand(
            "spawnCrashObject", {},
            [this](const cxxopts::ParseResult &result) {
                SpawnCrashObject();
            },
            "spawn a crash object");

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
            "give weapon");

        /*gApplication->_commandProcessor->RegisterCommand(
            "camFPV", {},
            [this](const cxxopts::ParseResult &result) {
                static bool fpvOn = false;
                fpvOn = !fpvOn;
                MafiaMP::Game::Helpers::Camera::SetFPV(fpvOn);
            },
            "toggles camera first person view");*/

        gApplication->_commandProcessor->RegisterCommand(
            "showEntityBrowser", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleEntityBrowser();
            },
            "toggles entity browser dialog");

        gApplication->_commandProcessor->RegisterCommand(
            "showAudioDebug", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleAudioDebug();
            },
            "toggles audio debug dialog");

        gApplication->_commandProcessor->RegisterCommand(
            "showCameraStudio", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleCameraStudio();
            },
            "toggles camera studio dialog");

        gApplication->_commandProcessor->RegisterCommand(
            "showPlayerDebug", {},
            [this](const cxxopts::ParseResult &result) {
                TogglePlayerDebug();
            },
            "toggles player debug dialog");

        gApplication->_commandProcessor->RegisterCommand(
            "showVehicleDebug", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleVehicleDebug();
            },
            "toggles vehicle debug dialog");

        gApplication->_commandProcessor->RegisterCommand(
            "showWorldDebug", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleWorldDebug();
            },
            "toggles world debug dialog");

        gApplication->_commandProcessor->RegisterCommand(
            "showNetworkStats", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleNetworkStats();
            },
            "toggles network stats dialog");
    }

    void DevFeatures::SetupMenuBar() {
        gApplication->_console->RegisterMenuBarDrawer([this]() {
            if (ImGui::BeginMenu("Debug")) {
                if (ImGui::MenuItem("Spawn car")) {
                    _worldDebug->SpawnCar();
                }
                if (ImGui::MenuItem("Spawn 50 cars")) {
                    for (size_t i = 0; i < 50; i++) {
                        _worldDebug->SpawnCar();
                    }
                }
                if (ImGui::MenuItem("Disconnect")) {
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
                if (ImGui::MenuItem("World debug", "F1")) {
                    ToggleWorldDebug();
                }
                if (ImGui::MenuItem("Player debug", "F2")) {
                    TogglePlayerDebug();
                }
                if (ImGui::MenuItem("Vehicle debug", "F3")) {
                    ToggleVehicleDebug();
                }
                if (ImGui::MenuItem("Camera Studio", "F4")) {
                    ToggleCameraStudio();
                }
                if (ImGui::MenuItem("Audio debug")) {
                    ToggleAudioDebug();
                }
                if (ImGui::MenuItem("Network stats", "F10")) {
                    ToggleNetworkStats();
                }
                if (ImGui::MenuItem("Entity Browser", "F11")) {
                    ToggleEntityBrowser();
                }

                ImGui::EndMenu();
            }
        });
    }

    void DevFeatures::ToggleAudioDebug() {
        _audioDebug->SetVisible(!_audioDebug->IsVisible());
    }

    void DevFeatures::ToggleEntityBrowser() {
        _entityBrowser->SetVisible(!_entityBrowser->IsVisible());
    }

    void DevFeatures::ToggleNetworkStats() {
        _networkStats->SetVisible(!_networkStats->IsVisible());
    }

    void DevFeatures::ToggleCameraStudio() {
        _cameraStudio->SetVisible(!_playerDebug->IsVisible());
    }

    void DevFeatures::TogglePlayerDebug() {
        _playerDebug->SetVisible(!_playerDebug->IsVisible());
    }

    void DevFeatures::ToggleVehicleDebug() {
        _vehicleDebug->SetVisible(!_vehicleDebug->IsVisible());
    }

    void DevFeatures::ToggleWorldDebug() {
        _worldDebug->SetVisible(!_worldDebug->IsVisible());
    }

    void DevFeatures::Disconnect() {
        gApplication->GetNetworkingEngine()->GetNetworkClient()->Disconnect();
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

    void DevFeatures::SpawnCrashObject() {
        auto info = Core::gApplication->GetEntityFactory()->RequestCrashObject("lh_city_stairs_module_a_v1");

        const auto OnCrashObjRequestFinished = [&](Game::Streaming::EntityTrackingInfo *info, bool success) {
            if (success) {
                auto crashObj = reinterpret_cast<SDK::C_CrashObj *>(info->GetEntity());
                if (!crashObj) {
                    return;
                }
                crashObj->GameInit();
                crashObj->Activate();

                auto localPlayer = SDK::GetGame()->GetActivePlayer();

                SDK::ue::sys::math::C_Vector newPos    = localPlayer->GetPos();
                SDK::ue::sys::math::C_Quat newRot      = localPlayer->GetRot();
                SDK::ue::sys::math::C_Matrix transform = {};
                transform.Identity();
                transform.SetRot(newRot);
                transform.SetPos(newPos);
                crashObj->SetTransform(transform);
            }
        };

        const auto OnCrashObjReturned = [&](Game::Streaming::EntityTrackingInfo *info, bool wasCreated) {
            if (!info) {
                return;
            }
            auto crashObj = reinterpret_cast<SDK::C_CrashObj *>(info->GetEntity());
            if (wasCreated && crashObj) {
                crashObj->Deactivate();
                crashObj->GameDone();
                crashObj->Release();
            }
        };

        info->SetRequestFinishCallback(OnCrashObjRequestFinished);
        info->SetReturnCallback(OnCrashObjReturned);
    }
} // namespace MafiaMP::Core
