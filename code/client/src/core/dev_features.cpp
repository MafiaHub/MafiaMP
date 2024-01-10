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
#include "sdk/entities/c_crash_obj.h"
#include "sdk/entities/c_vehicle.h"
#include "sdk/entities/human/c_human_script.h"
#include "sdk/mafia/framework/c_mafia_framework_interfaces.h"
#include "sdk/c_game_traffic_module.h"
#include "sdk/mafia/framework/director/c_game_director.h"
#include "sdk/mafia/ui/c_game_gui_2_module.h"

#include "game/helpers/ui.h"

#include "external/imgui/widgets/corner_text.h"

#include <cppfs/FileHandle.h>
#include <cppfs/fs.h>

#include "shared/rpc/chat_message.h"
#include "shared/rpc/spawn_car.h"

#include "shared/game_rpc/human/human_changeskin.h"

#include "modules/human.h"
#include "../sdk/ue/sys/sodb/c_sys_odb.h"
#include "../sdk/mafia/framework/c_vehicles_database.h"
#include "../sdk/mafia/framework/c_mafia_dbs.h"

namespace MafiaMP::Core {
    DevFeatures::DevFeatures() {
        _entityBrowser = std::make_shared<UI::EntityBrowser>();
        _cameraStudio  = std::make_shared<UI::CameraStudio>();
        _networkStats  = std::make_shared<UI::NetworkStats>();
    }

    void DevFeatures::Init() {
        
        // for 'spawn random car' feature
        std::srand(time(NULL));

        SetupCommands();
        SetupMenuBar();
    }

    void DevFeatures::Update() {
        if (_showEntityBrowser) {
            _entityBrowser->Update();
        }

        if (_showCameraStudio) {
            _cameraStudio->Update();
        }

        if (_showVehicledebug) {
            _vehicleDebug->Update();
        }

        if (_showNetworkStats) {
            _networkStats->Update();
        }

        if (gApplication->_input->IsKeyPressed(FW_KEY_F7)) {
            gApplication->GetImGUI()->ShowCursor(!_showCameraStudio);
            MafiaMP::Game::Helpers::Controls::Lock(!_showCameraStudio);
            _showCameraStudio = !_showCameraStudio;
        }

        if (gApplication->_input->IsKeyPressed(FW_KEY_F1)) {
            const auto human = Game::Helpers::Controls::GetLocalPlayer();
            if (human) {
                const auto districtHashName = SDK::mafia::framework::director::C_GameDirector::GetInstance()->GetDistrict(human->GetPos());
                const auto result           = *districtHashName;
            }
        }

        if (gApplication->_input->IsKeyPressed(FW_KEY_F2)) {
            SpawnCrashObject();
        }

        if (gApplication->_input->IsKeyPressed(FW_KEY_F11)) {
            ToggleEntityBrowser();
        }

        if (gApplication->_input->IsKeyPressed(FW_KEY_F10)) {
            ToggleNetworkStats();
        }

        if (gApplication->_input->IsKeyPressed(FW_KEY_F3)) {
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

                    const auto ent                         = info->GetNetworkEntity();
                    auto localPlayer                       = SDK::GetGame()->GetActivePlayer();

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

        if (gApplication->_input->IsKeyPressed(FW_KEY_F4)) {
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

    void DevFeatures::ToggleCameraStudio() {
        _showCameraStudio = !_showCameraStudio;
    }

    void DevFeatures::ToggleVehicleDebug() {
        _showVehicledebug = !_showVehicledebug;
    }

    void DevFeatures::ToggleNetworkStats() {
        _showNetworkStats = !_showNetworkStats;
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
                    car->GetVehicle()->SetSPZText("DEBUG", true);
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

    void DevFeatures::SpawnRandomCar() {
        auto mafiaDB = SDK::mafia::framework::GetMafiaDBs();
        if (!mafiaDB)
        {
            return;
        }

        auto vehiclesDB = mafiaDB->GetVehiclesDatabase();
        if (!vehiclesDB.m_Ptr)
        {
            return;
        }

        const uint32_t randomIndex = rand() % vehiclesDB.m_Ptr->GetVehiclesCount();

        using namespace SDK::mafia::framework;
        const C_VehiclesDatabase::TItemAccessorConst &carByIndex = vehiclesDB.m_Ptr->GetVehicleByIndex(randomIndex);
        if (const S_VehiclesTableItem* vehicle = carByIndex.Get()) // dud index
        {
            const char *modelName = &vehicle->m_ModelName[0];

            constexpr uint32_t TVF_CAR = (uint32_t)SDK::mafia::traffic::E_TrafficVehicleFlags::E_TVF_CAR;
            if (((uint32_t)vehicle->m_VehicleFlags & TVF_CAR) != TVF_CAR) {

                Framework::Logging::GetLogger("Debug")->info("Skipping {}, not E_TVF_CAR", modelName);
                return;
            }
            
            SpawnCar(modelName);

            Framework::Logging::GetLogger("Debug")->info("Spawned {}", modelName);
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
            "test", {},
            [this](cxxopts::ParseResult &) {

            },
            "crashes the game");
        gApplication->_commandProcessor->RegisterCommand(
            "weapon", {},
            [this](cxxopts::ParseResult&) {
                auto localPlayer = SDK::GetGame()->GetActivePlayer();
                localPlayer->GetInventoryWrapper()->AddWeapon(84, 300);
            },
            "Gives a random weapon to the player"
        );
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
            "spawnCrashObject", {},
            [this](const cxxopts::ParseResult &result) {
                SpawnCrashObject();
            },
            "spawn a crash object");
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
            "skin", {{"s,skin", "spawnprofile id", cxxopts::value<uint64_t>()->default_value("335218123840277515")}},
            [this](const cxxopts::ParseResult &result) {
                const auto human = Game::Helpers::Controls::GetLocalPlayer();
                if (human) {
                    const auto skinId = result["skin"].as<uint64_t>();
                    Framework::Logging::GetLogger("test")->debug("send rpc to change skin to {}", skinId);

                    FW_SEND_CLIENT_COMPONENT_GAME_RPC(Shared::RPC::HumanChangeSkin, gApplication->GetLocalPlayer(), skinId);
                }
            },
            "sends a chat message");
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
            "showCameraStudio", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleCameraStudio();
            },
            "toggles camera studio dialog");
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
                if (ImGui::MenuItem("Spawn car", "F1")) {
                    SpawnCar();
                }
                if (ImGui::MenuItem("Spawn 50 cars", "F1")) {
                    for (size_t i = 0; i < 50; i++)
                    {
                        SpawnCar();
                    }
                }
                if (ImGui::MenuItem("Spawn random car")) {
                    SpawnRandomCar();
                }
                if (ImGui::MenuItem("Spawn 50 random cars")) {
                    for (size_t i = 0; i < 50; i++) {
                        SpawnRandomCar();
                    }
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
                if (ImGui::MenuItem("Entity Browser", "F11")) {
                    ToggleEntityBrowser();
                }
                if (ImGui::MenuItem("Camera Studio")) {
                    ToggleCameraStudio();
                }
                if (ImGui::MenuItem("Vehicle debug")) {
                    ToggleVehicleDebug();
                }
                if (ImGui::MenuItem("Network stats", "F10")) {
                    ToggleNetworkStats();
                }
                if (ImGui::MenuItem("Database Test")) {
                    SDK::mafia::framework::C_MafiaDBs *mafiaDB = SDK::mafia::framework::GetMafiaDBs();
                    auto vehiclesDB = mafiaDB->GetVehiclesDatabase();
                    const uint32_t NumVehicles                          = vehiclesDB.m_Ptr->GetVehiclesCount();
                    
                    using namespace SDK::mafia::framework;
                    using namespace SDK::ue::sys::utils;

                    const C_VehiclesDatabase::TItemAccessorConst& carByIndex = vehiclesDB.m_Ptr->GetVehicleByIndex(1);
                    const C_VehiclesDatabase::TItemAccessorConst &carByID    = vehiclesDB.m_Ptr->GetVehicleByID(1);

                    const uint64_t carHash = C_HashName::ComputeHash("bolt_truck");
                    const C_VehiclesDatabase::TItemAccessorConst &carByHash = vehiclesDB.m_Ptr->GetVehicleByModel(carHash);

                    int z = 0;
                }

                ImGui::EndMenu();
            }
        });
    }

} // namespace MafiaMP::Core
