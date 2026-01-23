#include "dev_features.h"

#include <cppfs/FileHandle.h>
#include <cppfs/fs.h>
#include <imgui/imgui.h>

#include "application.h"

#include <logging/logger.h>

#include "game/helpers/controls.h"
#include "game/helpers/human.h"
#include "game/overrides/character_controller.h"
#include "game/streaming/entity_factory.h"

#include "sdk/entities/c_crash_obj.h"
#include "sdk/entities/c_player_2.h"
#include "sdk/mafia/framework/c_mafia_framework_interfaces.h"

#include "shared/rpc/chat_message.h"

namespace MafiaMP::Core {
    DevFeatures::DevFeatures() {
        _cameraStudio  = std::make_shared<UI::Devs::CameraStudio>();
        _debugAudio    = std::make_shared<UI::Devs::DebugAudio>();
        _debugPlayer   = std::make_shared<UI::Devs::DebugPlayer>();
        _debugPrefab   = std::make_shared<UI::Devs::DebugPrefab>();
        _debugVehicle  = std::make_shared<UI::Devs::DebugVehicle>();
        _debugWorld    = std::make_shared<UI::Devs::DebugWorld>();
        _entityBrowser = std::make_shared<UI::Devs::EntityBrowser>();
        _networkStats  = std::make_shared<UI::Devs::NetworkStats>();
    }

    void DevFeatures::Init() {
        SetupCommands();
        SetupMenuBar();
    }

    void DevFeatures::Update() {
        _cameraStudio->Update();
        _debugAudio->Update();
        _debugPlayer->Update();
        _debugPrefab->Update();
        _debugVehicle->Update();
        _debugWorld->Update();
        _entityBrowser->Update();
        _networkStats->Update();

        /**
         * F1 is reserved for bypassing lock controls
         * F8 is reserved for console
         * F9 is reserved for main menu
         * F12 is reserved for Steam screenshot
         */
        if (gApplication->GetInput()->IsKeyPressed(FW_KEY_F2)) {
            ToggleDebugWorld();
        }

        if (gApplication->GetInput()->IsKeyPressed(FW_KEY_F3)) {
            ToggleDebugPlayer();
        }

        if (gApplication->GetInput()->IsKeyPressed(FW_KEY_F4)) {
            ToggleDebugVehicle();
        }

        if (gApplication->GetInput()->IsKeyPressed(FW_KEY_F5)) {
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
                    if (auto wepCtrl = human->GetHumanWeaponController()) {
                        wepCtrl->DoWeaponSelectByItemId(85, true);
                    }
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

        if (gApplication->GetInput()->IsKeyPressed(FW_KEY_F6)) {
            if (!_TEMP_HUMAN) {
                return;
            }

            const auto human = reinterpret_cast<SDK::C_Human2 *>(_TEMP_HUMAN->GetEntity());

            auto localPlayer = SDK::GetGame()->GetActivePlayer();

            SDK::ue::sys::math::C_Vector newPos = localPlayer->GetPos();

            SDK::ue::C_CntPtr<uintptr_t> syncObject2;
            SDK::C_Entity *ent = reinterpret_cast<SDK::C_Entity *>(localPlayer);
            if (auto wepCtrl = human->GetHumanWeaponController()) {
                human->GetHumanScript()->ScrAim(syncObject2, !wepCtrl->IsAiming());
                // human->GetHumanScript()->ScrAimAt(syncObject2, ent, newPos, !wepCtrl->IsAiming());
                Framework::Logging::GetLogger("Playground")->debug("Aiming : {}", wepCtrl->IsAiming());
            }
        }

        if (gApplication->GetInput()->IsKeyPressed(FW_KEY_F7)) {
            SpawnCrashObject();
        }

        if (gApplication->GetInput()->IsKeyPressed(FW_KEY_F10)) {
            ToggleNetworkStats();
        }

        if (gApplication->GetInput()->IsKeyPressed(FW_KEY_F11)) {
            ToggleEntityBrowser();
        }
    }

    void DevFeatures::Shutdown() {}

    void DevFeatures::SetupCommands() {
        gApplication->GetCommandProcessor()->RegisterCommand(
            "help", {},
            [this](cxxopts::ParseResult &) {
                std::stringstream ss;
                for (const auto &name : gApplication->GetCommandProcessor()->GetCommandNames()) {
                    ss << fmt::format("{} {:>8}\n", name, gApplication->GetCommandProcessor()->GetCommandInfo(name)->options->help());
                }
                Framework::Logging::GetLogger("Debug")->info("Available commands:\n{}", ss.str());
            },
            "prints all available commands");

        gApplication->GetCommandProcessor()->RegisterCommand(
            "exit", {},
            [this](cxxopts::ParseResult &) {
                CloseGame();
            },
            "quits the game");

        gApplication->GetCommandProcessor()->RegisterCommand(
            "disconnect", {},
            [this](const cxxopts::ParseResult &) {
                Disconnect();
            },
            "disconnect from server");

        gApplication->GetCommandProcessor()->RegisterCommand(
            "lua", {{"c,command", "command to execute", cxxopts::value<std::string>()->default_value("")}, {"f,file", "file to execute", cxxopts::value<std::string>()->default_value("")}},
            [this](const cxxopts::ParseResult &result) {
                std::string command = result["command"].as<std::string>();
                if (!command.empty()) {
                    gApplication->GetLuaVM()->ExecuteString(command.c_str());
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
                    gApplication->GetLuaVM()->ExecuteString(content.c_str());
                }
            },
            "executes Lua commands");

        gApplication->GetCommandProcessor()->RegisterCommand(
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

        gApplication->GetCommandProcessor()->RegisterCommand(
            "spawnCar", {{"m,model", "model name of the car", cxxopts::value<std::string>()->default_value("berkley_810")}},
            [this](const cxxopts::ParseResult &result) {
                std::string modelName = result["model"].as<std::string>();
                _debugWorld->SpawnCar(modelName);
            },
            "spawn a car of a given model");

        gApplication->GetCommandProcessor()->RegisterCommand(
            "spawnCrashObject", {},
            [this](const cxxopts::ParseResult &result) {
                SpawnCrashObject();
            },
            "spawn a crash object");

        gApplication->GetCommandProcessor()->RegisterCommand(
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

        gApplication->GetCommandProcessor()->RegisterCommand(
            "showCameraStudio", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleCameraStudio();
            },
            "toggles camera studio dialog");

        gApplication->GetCommandProcessor()->RegisterCommand(
            "showDebugAudio", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleDebugAudio();
            },
            "toggles audio debug dialog");

        gApplication->GetCommandProcessor()->RegisterCommand(
            "showDebugPlayer", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleDebugPlayer();
            },
            "toggles player debug dialog");

        gApplication->GetCommandProcessor()->RegisterCommand(
            "showDebugPrefab", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleDebugPrefab();
            },
            "toggles prefab manager dialog");

        gApplication->GetCommandProcessor()->RegisterCommand(
            "showDebugVehicle", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleDebugVehicle();
            },
            "toggles vehicle debug dialog");

        gApplication->GetCommandProcessor()->RegisterCommand(
            "showDebugWorld", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleDebugWorld();
            },
            "toggles world debug dialog");

        gApplication->GetCommandProcessor()->RegisterCommand(
            "showEntityBrowser", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleEntityBrowser();
            },
            "toggles entity browser dialog");

        gApplication->GetCommandProcessor()->RegisterCommand(
            "showNetworkStats", {},
            [this](const cxxopts::ParseResult &result) {
                ToggleNetworkStats();
            },
            "toggles network stats dialog");
    }

    void DevFeatures::SetupMenuBar() {
        gApplication->GetConsole()->RegisterMenuBarDrawer([this]() {
            if (ImGui::BeginMenu("Quick commands")) {
                if (ImGui::MenuItem("Spawn car")) {
                    _debugWorld->SpawnCar();
                }
                if (ImGui::MenuItem("Spawn 50 cars")) {
                    for (size_t i = 0; i < 50; i++) {
                        _debugWorld->SpawnCar();
                    }
                }
                if (ImGui::MenuItem("Re-download assets")) {
                    RedownloadAssets();
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

            if (ImGui::BeginMenu("Dialogs")) {
                if (ImGui::MenuItem("Audio debug")) {
                    ToggleDebugAudio();
                }
                if (ImGui::MenuItem("Player debug", "F3")) {
                    ToggleDebugPlayer();
                }
                if (ImGui::MenuItem("Prefab Manager")) {
                    ToggleDebugPrefab();
                }
                if (ImGui::MenuItem("Vehicle debug", "F4")) {
                    ToggleDebugVehicle();
                }
                if (ImGui::MenuItem("World debug", "F2")) {
                    ToggleDebugWorld();
                }
                if (ImGui::MenuItem("Camera Studio")) {
                    ToggleCameraStudio();
                }
                if (ImGui::MenuItem("Entity Browser", "F11")) {
                    ToggleEntityBrowser();
                }
                if (ImGui::MenuItem("Network stats", "F10")) {
                    ToggleNetworkStats();
                }

                ImGui::EndMenu();
            }
        });
    }

    void DevFeatures::ToggleCameraStudio() {
        _cameraStudio->Toggle();
    }

    void DevFeatures::ToggleDebugAudio() {
        _debugAudio->Toggle();
    }

    void DevFeatures::ToggleDebugPlayer() {
        _debugPlayer->Toggle();
    }

    void DevFeatures::ToggleDebugPrefab() {
        _debugPrefab->Toggle();
    }

    void DevFeatures::ToggleDebugVehicle() {
        _debugVehicle->Toggle();
    }

    void DevFeatures::ToggleDebugWorld() {
        _debugWorld->Toggle();
    }

    void DevFeatures::ToggleEntityBrowser() {
        _entityBrowser->Toggle();
    }

    void DevFeatures::ToggleNetworkStats() {
        _networkStats->Toggle();
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
        // Cycle through different tree models from the SDS
        static const char *crashObjectModels[] = {
            "lh_tree_acer_saccharum_a_v1",
            "lh_tree_acer_saccharum_a_v2",
            "lh_tree_acer_saccharum_a_v3",
            "lh_tree_acer_saccharum_a_v4",
            "lh_tree_betula_papyrifera_a_v1",
            "lh_tree_betula_papyrifera_a_v2",
            "lh_tree_betula_papyrifera_a_v3",
            "lh_tree_gleditsia_triacanthos_a_v1",
            "lh_tree_gleditsia_triacanthos_a_v2",
            "lh_tree_gleditsia_triacanthos_a_v3",
            "lh_tree_gleditsia_triacanthos_a_v4",
            "lh_tree_magnolia_grandiflora_a_v1",
            "lh_tree_magnolia_grandiflora_a_v2",
            "lh_tree_magnolia_grandiflora_a_v3",
            "lh_tree_magnolia_grandiflora_a_v4",
            "lh_tree_pinus_resinosa_a_v1",
            "lh_tree_pinus_resinosa_a_v2",
            "lh_tree_pinus_resinosa_a_v3",
            "lh_tree_pinus_resinosa_a_v4",
            "lh_tree_pinus_sylvestris_a_v1",
            "lh_tree_pinus_sylvestris_a_v2",
            "lh_tree_populus_nigra_a_v1",
            "lh_tree_populus_nigra_a_v2",
            "lh_tree_prunus_serotina_a_v1",
            "lh_tree_prunus_serotina_a_v2",
            "lh_tree_prunus_serotina_a_v3",
            "lh_tree_quercus_alba_a_v1",
            "lh_tree_quercus_alba_a_v2",
            "lh_tree_quercus_alba_a_v3",
            "lh_tree_quercus_alba_a_v4",
            "lh_tree_quercus_alba_b_v2",
            "lh_tree_ulmus_americana_a_v1",
            "lh_tree_ulmus_americana_a_v2",
            "lh_tree_ulmus_americana_a_v3",
            "lh_tree_ulmus_americana_a_v4",
            "lh_tree_ulmus_americana_a_v6",
        };
        static size_t crashObjectIndex = 0;

        const char *modelName = crashObjectModels[crashObjectIndex];
        crashObjectIndex      = (crashObjectIndex + 1) % std::size(crashObjectModels);

        Framework::Logging::GetLogger("Playground")->info("Spawning crash object [{}/{}]: {}",
            crashObjectIndex, std::size(crashObjectModels), modelName);

        auto info = Core::gApplication->GetEntityFactory()->RequestCrashObject(modelName);

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
    void DevFeatures::RedownloadAssets() {
        Core::gApplication->DownloadsAssetsFromConnectedServer();
    }
} // namespace MafiaMP::Core
