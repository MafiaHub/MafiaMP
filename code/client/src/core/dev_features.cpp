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
        _localReplicator = std::make_unique<Dev::LocalReplicator>();
        _cameraStudio    = std::make_shared<UI::Devs::CameraStudio>();
        _debugAudio      = std::make_shared<UI::Devs::DebugAudio>();
        _debugPlayer     = std::make_shared<UI::Devs::DebugPlayer>();
        _debugPrefab     = std::make_shared<UI::Devs::DebugPrefab>();
        _debugVehicle    = std::make_shared<UI::Devs::DebugVehicle>();
        _debugWorld      = std::make_shared<UI::Devs::DebugWorld>();
        _entityBrowser   = std::make_shared<UI::Devs::EntityBrowser>();
        _networkStats    = std::make_shared<UI::Devs::NetworkStats>();
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
            // Toggle local replication
            if (_localReplicator->IsActive()) {
                _localReplicator->StopAndDespawn();
            }
            else {
                _localReplicator->SpawnAndStartReplication(10029431515544697714);
            }
        }

        // Update local replication each frame
        _localReplicator->Update();

        if (gApplication->GetInput()->IsKeyPressed(FW_KEY_F6)) {
            // Toggle aiming on the replicated human
            _localReplicator->ToggleAim();
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
        // Crash object models from entity browser dump
        static const char *crashObjectModels[] = {
            "_dlc3_bc_09_unique_sammys_doorboards_bv01_00", "_dlc3_bc_09_unique_sammys_doorboards_bv01_01", "_dlc3_bc_09_unique_sammys_doorboards_bv01_02",
            "bc_barrel_oil_a_v1_bv01_00", "bc_barrel_oil_a_v1_bv01_01", "bc_barrel_oil_a_v1_bv01_02", "bc_barrel_oil_a_v1_bv01_03", "bc_barrel_oil_a_v1_bv01_04",
            "bc_barrel_oil_small_a_v1_00", "bc_barrel_oil_small_a_v1_01", "bc_barrel_oil_small_a_v1_02", "bc_barrel_oil_small_a_v1_03", "bc_barrel_oil_small_a_v1_04",
            "bc_barrel_oil_small_a_v1_05", "bc_barrel_oil_small_a_v1_06", "bc_barrel_oil_small_a_v1_07", "bc_barrel_oil_small_a_v1_08", "bc_barrel_oil_small_a_v1_09",
            "bc_barrel_oil_small_a_v1_10", "bc_barrel_oil_small_a_v1_11", "bc_barrel_oil_small_a_v1_12", "bc_barrel_oil_small_a_v1_13", "bc_barrel_oil_small_a_v1_14",
            "bc_barrel_oil_small_a_v1_15", "bc_barrel_oil_small_a_v1_bv01_00", "bc_barrel_oil_small_a_v1_bv01_01", "bc_barrel_oil_small_a_v1_bv01_02", "bc_barrel_oil_small_a_v1_bv01_03",
            "bc_baseball_bat_a_bv01_00", "bc_baseball_bat_a_bv01_01", "bc_baseball_bat_a_bv01_02", "bc_baseball_bat_a_bv01_03", "bc_baseball_bat_a_bv01_04", "bc_baseball_bat_a_bv01_05", "bc_baseball_bat_a_bv01_06",
            "bc_ceiling_bowl_light_fixture_bv01_00", "bc_ceiling_disc_light_fixture_bv01_00",
            "bc_cinderblock_single_a_bv01_03", "bc_cinderblock_single_a_bv01_04", "bc_cinderblock_single_a_bv01_05", "bc_cinderblock_single_a_bv01_06", "bc_cinderblock_single_a_bv01_07",
            "bc_city_lamp_wall_b_bv01_00", "bc_city_lamp_wall_c_bv01_00",
            "bc_crate_content_army_wi_bv01_00", "bc_crate_content_army_wi_bv01_01", "bc_crate_content_army_wi_bv01_02", "bc_crate_content_army_wi_bv01_03", "bc_crate_content_army_wi_bv01_04",
            "bc_crate_content_army_wi_bv01_05", "bc_crate_content_army_wi_bv01_06", "bc_crate_content_army_wi_bv01_07", "bc_crate_content_army_wi_bv01_08", "bc_crate_content_army_wi_bv01_09",
            "bc_crate_content_army_wi_bv01_10", "bc_crate_content_army_wi_bv01_11", "bc_crate_content_army_wi_bv01_12", "bc_crate_content_army_wi_bv01_13", "bc_crate_content_army_wi_bv01_14",
            "bc_crate_content_army_wi_bv01_15", "bc_crate_content_army_wi_bv01_16", "bc_crate_content_army_wi_bv01_17", "bc_crate_content_army_wi_bv01_18", "bc_crate_content_army_wi_bv01_19",
            "bc_crate_content_army_wi_bv01_20", "bc_crate_content_army_wi_bv01_21", "bc_crate_content_army_wi_bv01_22", "bc_crate_content_army_wi_bv01_23", "bc_crate_content_army_wi_bv01_24",
            "bc_crate_content_army_wi_bv01_25", "bc_crate_content_army_wi_bv01_26", "bc_crate_content_army_wi_bv01_27", "bc_crate_content_army_wi_bv01_28", "bc_crate_content_army_wi_bv01_29",
            "bc_crate_content_army_wi_bv01_30", "bc_crate_content_army_wi_bv01_31", "bc_crate_content_army_wi_bv01_32", "bc_crate_content_army_wi_bv01_33", "bc_crate_content_army_wi_bv01_34",
            "bc_crate_content_army_wi_bv01_35", "bc_crate_content_army_wi_bv01_37", "bc_crate_content_army_wi_bv01_38", "bc_crate_content_army_wi_bv01_39",
            "bc_cutting_board_wood_bv01_02", "bc_cutting_board_wood_bv01_03", "bc_cutting_board_wood_bv01_04", "bc_cutting_board_wood_bv01_05", "bc_cutting_board_wood_bv01_06", "bc_cutting_board_wood_bv01_07",
            "bc_food_bowl_no_soup_bv01_00",
            "bc_loc_am_park_teddy_bear_bv03_00", "bc_loc_am_park_wooden_fence_c_bv01_03", "bc_loc_am_park_wooden_fence_c_bv01_27",
            "bc_loc_bakery_bottle_milk_old_bv01_00", "bc_loc_bakery_bottle_milk_old_bv01_01", "bc_loc_bakery_bottle_milk_old_bv01_02",
            "bc_loc_bakery_dough_breaker_v3_bv01_00", "bc_loc_bakery_shelf_d_bv02_00", "bc_loc_bakery_utensil_b_v1_bv01_01", "bc_loc_bakery_utensil_c_bv01_00",
            "bc_loc_bakeryshop_jar_a_bv01_00", "bc_loc_bakeryshop_jar_a_bv01_01", "bc_loc_bakeryshop_jar_a_bv01_02", "bc_loc_bakeryshop_jar_a_bv01_03", "bc_loc_bakeryshop_jar_a_bv01_04",
            "bc_loc_bakeryshop_jar_a_bv01_05", "bc_loc_bakeryshop_jar_a_bv01_06", "bc_loc_bakeryshop_jar_a_bv01_07", "bc_loc_bakeryshop_jar_a_bv01_08", "bc_loc_bakeryshop_jar_a_bv01_09",
            "bc_loc_bakeryshop_jar_a_bv01_10", "bc_loc_bakeryshop_jar_a_bv01_11",
            "bc_loc_bakeryshop_jar_b_bv01_00", "bc_loc_bakeryshop_jar_b_bv01_01", "bc_loc_bakeryshop_jar_b_bv01_02", "bc_loc_bakeryshop_jar_b_bv01_03", "bc_loc_bakeryshop_jar_b_bv01_04",
            "bc_loc_bakeryshop_jar_c_bv01_00", "bc_loc_bakeryshop_jar_c_bv01_01", "bc_loc_bakeryshop_jar_c_bv01_02", "bc_loc_bakeryshop_jar_c_bv01_03", "bc_loc_bakeryshop_jar_c_bv01_04",
            "bc_loc_bath_house_shelf_a_bv01_00", "bc_loc_bath_house_shelf_a_bv01_01", "bc_loc_burke_lamp_a_bv01_01",
            "bc_loc_cassandra_picture_big_a_no_joint_bv01_00", "bc_loc_cassandra_picture_big_a_no_joint_bv01_01", "bc_loc_cassandra_picture_big_a_no_joint_bv01_02",
            "bc_loc_cassandra_switch_a_bv01_00", "bc_loc_cassandra_switch_a_bv01_01", "bc_loc_cassandra_switch_a_bv01_02", "bc_loc_cassandra_switch_a_bv01_03",
            "bc_loc_cassandra_switch_a_bv01_04", "bc_loc_cassandra_switch_a_bv01_05", "bc_loc_cassandra_switch_a_bv01_06", "bc_loc_cassandra_switch_a_bv01_07",
            "bc_loc_father_james_ceiling_fan_v2_bv01_00", "bc_loc_father_james_church__cabinet_b_bv01_00",
            "bc_loc_father_james_church_kitchen_pot_v3_bv01_00", "bc_loc_father_james_church_kitchen_pot_v3_bv01_02", "bc_loc_father_james_picture_d_bv01_01",
            "bc_loc_gas_station_goods_g_bv01_00", "bc_loc_gas_station_goods_g_bv01_01", "bc_loc_gas_station_goods_g_bv01_02",
            "bc_loc_grocery_store_box_cake_bv01_00", "bc_loc_grocery_store_box_soap_b_bv01_00",
            "bc_loc_grocery_store_can_corn_bv01_00", "bc_loc_grocery_store_can_corn_bv01_01", "bc_loc_grocery_store_can_corn_bv01_02", "bc_loc_grocery_store_can_corn_bv01_03", "bc_loc_grocery_store_can_corn_bv01_04",
            "bc_loc_grocery_store_can_peas_bv01_00", "bc_loc_grocery_store_can_peas_bv01_01", "bc_loc_grocery_store_can_peas_bv01_02", "bc_loc_grocery_store_can_peas_bv01_03", "bc_loc_grocery_store_can_peas_bv01_04",
            "bc_loc_grocery_store_knife_rack_bv01_01", "bc_loc_grocery_store_knife_rack_bv01_02", "bc_loc_grocery_store_meat_grinder_bv01_00",
            "bc_loc_grocery_store_meat_scale_bv01_00", "bc_loc_grocery_store_minced_meat_pan_small_bv01_00",
            "bc_loc_jazz_club_beertray_a_bv01_00", "bc_loc_jazz_club_bottle_beer_a_00", "bc_loc_jazz_club_bottle_wine_a_bv01_00", "bc_loc_jazz_club_bottle_wine_b_bv01_00",
            "bc_loc_jazz_club_bottles_spirits_a_bv01_00", "bc_loc_jazz_club_bottles_wine_a_bv01_00", "bc_loc_jazz_club_bottles_wine_a_bv01_01",
            "bc_loc_jazz_club_cabinet_a_bv01_00", "bc_loc_jazz_club_cabinet_a_bv01_01", "bc_loc_jazz_club_chair_a", "bc_loc_jazz_club_faucets_a_bv01_00",
            "bc_loc_jazz_club_hanging_lamp_a", "bc_loc_jazz_club_hanging_lamp_a_burned_v3_00_bv_00", "bc_loc_jazz_club_table_c_bv01_00", "bc_loc_jazz_club_table_c_bv01_01",
            "bc_loc_kill_olivia_champagnebottle_bv01_00",
            "bc_loc_kill_tony_fl_book_set_a_bv01_01", "bc_loc_kill_tony_fl_book_set_a_bv01_02", "bc_loc_kill_tony_fl_book_set_a_bv01_03", "bc_loc_kill_tony_fl_luggage_a_bv01",
            "bc_loc_kill_tony_levilorblind_e_bv01_00", "bc_loc_kill_tony_levilorblind_e_bv01_01", "bc_loc_kill_tony_levilorblind_e_bv01_02",
            "bc_loc_kill_tony_levilorblind_e_bv01_05", "bc_loc_kill_tony_levilorblind_e_bv01_06",
            "bc_loc_kill_tony_ts_lamp_a", "bc_loc_kill_tony_ts_lamp_b_bv01_00",
            "bc_loc_pillow_bv01_00", "bc_loc_sammys_wardrobe_interior_a_bv01_00", "bc_loc_slaughterhouse_board_bv01_00", "bc_loc_slaughterhouse_metal_pans_a_bv01_00",
            "bc_loc_sports_bar_wooden_railing_b_bv01_00", "bc_loc_strip_club_end_table_bv01_00", "bc_loc_strip_club_end_table_bv01_01",
            "bc_pedestrian_book_closed_bv01_00",
            "bc_porno_poster_a_bv01_00", "bc_porno_poster_a_bv01_01", "bc_porno_poster_a_bv01_02",
            "bc_porno_poster_c_bv01_02", "bc_porno_poster_c_bv01_03", "bc_porno_poster_c_bv01_04", "bc_porno_poster_c_bv01_05", "bc_porno_poster_c_bv01_06",
            "bc_porno_poster_c_bv01_08", "bc_porno_poster_c_bv01_09", "bc_porno_poster_c_bv01_13",
            "bc_power_pole_e_00", "bc_push_broom_bv01_00", "bc_sauce_pan_a_bv01_00", "bc_sauce_pan_a_bv01_02",
            "bc_shop_win_drugstore_pot_a_bv01_00", "bc_shop_win_drugstore_pot_a_bv01_01",
            "bc_sign_desk_a_var_o_bv01_00", "bc_sign_hat_store_a_bv01_00", "bc_sign_shop_b_bv01_00",
            "bc_small_trash_bin_bv01_00", "bc_small_trash_bin_bv01_01", "bc_stirring_pipe_bv01_00",
            "bc_tire_stack_a_bv01", "bc_tire_stack_a_tall_bv01",
            "bc_wall_alley_curve_light_fixture_bv01_00", "bc_wall_alley_light_fixture_bv01_00",
            "bc_wooden_box_a_04", "bc_wooden_box_a_2x1_bv01_00", "bc_wooden_box_a_2x2_bv01", "bc_wooden_crates_c_kit_bv01",
            "bc_woodpole_a_bv01_00", "bc_woodpole_a_bv01_01",
            "billboard_01", "billboard_02", "c_wooden_box_group_a_00",
            "dlc1_bc_loc_pot_farm_greenhouse_wooden_panel_a_bv01_00", "dlc1_bc_loc_pot_farm_greenhouse_wooden_panel_b_bv01_00",
            "dlc1_bc_loc_pot_farm_greenhouse_wooden_panel_b_bv01_01", "dlc1_bc_loc_pot_farm_greenhouse_wooden_panel_b_bv01_02",
            "dlc1_rag_a_bv01_00", "dlc1_rag_a_bv01_01", "dlc1_rag_a_bv01_02",
            "dlc1_sheriff_hq_wanted_poster_b_00", "dlc1_sheriff_hq_wanted_poster_b_02", "dlc1_sheriff_hq_wanted_poster_b_04", "dlc1_sheriff_hq_wanted_poster_b_05",
            "dlc1_sheriff_hq_wanted_poster_b_06", "dlc1_sheriff_hq_wanted_poster_b_07", "dlc1_sheriff_hq_wanted_poster_b_10", "dlc1_sheriff_hq_wanted_poster_b_11",
            "dlc1_sheriff_hq_wanted_poster_b_12", "dlc1_sheriff_hq_wanted_poster_b_13", "dlc1_sheriff_hq_wanted_poster_b_14", "dlc1_sheriff_hq_wanted_poster_b_15",
            "dlc1_sheriff_hq_wanted_poster_b_16", "dlc1_sheriff_hq_wanted_poster_b_17", "dlc1_sheriff_hq_wanted_poster_b_18", "dlc1_sheriff_hq_wanted_poster_b_19",
            "dlc1_sheriff_hq_wanted_poster_b_20", "dlc1_sheriff_hq_wanted_poster_b_21", "dlc1_sheriff_hq_wanted_poster_b_22", "dlc1_sheriff_hq_wanted_poster_b_23",
            "dlc1_watertrough_a_bv01_00",
            "dlc3_abandoned_picture_family_a_bv01_00", "dlc3_abandoned_picture_family_a_bv01_01", "dlc3_abandoned_picture_family_a_bv01_02", "dlc3_abandoned_picture_family_a_bv01_03",
            "dlc3_knife_v1_bv01_00", "dlc3_knife_v1_bv01_01",
            "eb_fence_wood_h_post_a_0", "entity_trans-eb_00_sewer_pipes_b00",
            "lh_06_street_lamp_a_v1_bv01_00",
            "lh_advert_generic_a_v1_bv01", "lh_advert_generic_c_v1", "lh_advert_generic_h_v1", "lh_advert_generic_i_v1_bv01_00",
            "lh_advert_generic_o_v1", "lh_advert_generic_q_v1", "lh_advert_generic_r_v1", "lh_advert_generic_t_v1",
            "lh_advert_generic_u_v1_bv01", "lh_advert_generic_w_v1_bv01", "lh_advert_long_e_v1_bv01_00",
            "lh_advert_street_board_a_v1", "lh_advert_street_board_a_v1_bv01_00",
            "lh_ammo_pack_a_v1_bv01_00", "lh_ammo_pack_a_v1_bv01_01",
            "lh_apple_a_v1_bv01_00", "lh_apple_a_v1_bv01_01", "lh_apple_a_v1_bv01_02", "lh_apple_a_v1_bv01_03", "lh_apple_a_v1_bv01_04",
            "lh_apple_a_v1_bv01_05", "lh_apple_a_v1_bv01_06", "lh_apple_a_v1_bv01_07", "lh_apple_a_v1_bv01_08",
            "lh_ashtray_b_v1_bv01_00", "lh_ashtray_b_v1_bv01_01", "lh_ashtray_b_v1_bv01_02", "lh_ashtray_b_v1_bv01_03", "lh_ashtray_b_v1_bv01_04", "lh_ashtray_b_v1_bv01_05",
            "lh_ashtray_b_v2_00", "lh_ashtray_b_v2_01", "lh_ashtray_b_v2_bv01_00", "lh_ashtray_b_v2_bv01_01", "lh_ashtray_b_v2_bv01_02", "lh_ashtray_b_v2_bv01_03", "lh_ashtray_b_v2_bv01_04",
            "lh_ashtray_b_v2_bv01_09", "lh_ashtray_b_v2_bv01_10", "lh_ashtray_b_v2_bv01_11", "lh_ashtray_b_v2_bv01_12", "lh_ashtray_b_v2_bv01_13", "lh_ashtray_b_v2_bv01_14", "lh_ashtray_b_v2_bv01_15", "lh_ashtray_b_v2_bv01_16",
            "lh_ashtray_b_v3_bv01_00", "lh_ashtray_b_v3_bv01_01", "lh_ashtray_b_v3_bv01_02", "lh_ashtray_b_v3_bv01_03", "lh_ashtray_b_v3_bv01_04",
            "lh_bakery_bags_b_v1_bv01_01",
            "lh_bakery_bags_d_v1_bv01_00", "lh_bakery_bags_d_v1_bv01_01", "lh_bakery_bags_d_v1_bv01_02", "lh_bakery_bags_d_v1_bv01_05", "lh_bakery_bags_d_v1_bv01_06",
            "lh_bakery_bags_d_v1_bv01_07", "lh_bakery_bags_d_v1_bv01_08", "lh_bakery_bags_d_v1_bv01_09", "lh_bakery_bags_d_v1_bv01_10", "lh_bakery_bags_d_v1_bv01_11", "lh_bakery_bags_d_v1_bv01_12", "lh_bakery_bags_d_v1_bv01_13",
            "lh_bakery_basket_a_v1_bv01_00", "lh_bakery_basket_a_v1_bv01_01", "lh_bakery_basket_a_v1_bv01_02", "lh_bakery_basket_a_v1_bv01_03", "lh_bakery_basket_a_v1_bv01_04",
            "lh_bakery_basket_a_v1_bv01_05", "lh_bakery_basket_a_v1_bv01_06", "lh_bakery_basket_a_v1_bv01_07", "lh_bakery_basket_a_v1_bv01_08", "lh_bakery_basket_a_v1_bv01_09",
            "lh_bakery_basket_a_v1_bv01_10", "lh_bakery_basket_a_v1_bv01_11", "lh_bakery_basket_a_v1_bv01_12", "lh_bakery_basket_a_v1_bv01_13", "lh_bakery_basket_a_v1_bv01_14",
            "lh_bakery_basket_a_v1_bv01_15", "lh_bakery_basket_a_v1_bv01_16", "lh_bakery_basket_a_v1_bv01_17", "lh_bakery_basket_a_v1_bv01_18", "lh_bakery_basket_a_v1_bv01_19",
            "lh_bakery_basket_a_v1_bv01_20", "lh_bakery_basket_a_v1_bv01_21", "lh_bakery_basket_a_v1_bv01_22", "lh_bakery_basket_a_v1_bv01_23", "lh_bakery_basket_a_v1_bv01_24",
            "lh_bakery_basket_b_v1_bv01_00", "lh_bakery_basket_b_v1_bv01_01", "lh_bakery_basket_b_v1_bv01_02", "lh_bakery_basket_b_v1_bv01_03", "lh_bakery_basket_b_v1_bv01_04",
            "lh_bakery_basket_b_v1_bv01_05", "lh_bakery_basket_b_v1_bv01_06", "lh_bakery_basket_b_v1_bv01_07", "lh_bakery_basket_b_v1_bv01_08", "lh_bakery_basket_b_v1_bv01_09", "lh_bakery_basket_b_v1_bv01_10",
            "lh_bakery_basket_c_v1_bv01_00", "lh_bakery_basket_c_v1_bv01_01", "lh_bakery_basket_c_v1_bv01_02",
            "lh_bakery_box_open_a_v1_bv01_00", "lh_bakery_chalkboard_a_v1", "lh_barbers_pole_v1_bv01_00",
            "lh_barrel_a_v1", "lh_barrel_a_v1_bv01_00", "lh_barrel_a_v1_bv01_01", "lh_barrel_a_v1_bv01_02",
            "lh_barrel_fire_a_v1_bv01_00", "lh_barrel_fire_a_v1_bv01_01", "lh_barrel_fire_a_v1_bv01_02", "lh_barrel_trash_can_a_v1",
            "lh_bed_frame_mattress_a_v1_bv01_00", "lh_bed_frame_mattress_a_v1_bv01_01",
            "lh_beer_bottle_a_v1", "lh_beer_bottle_a_v1_bv01_00", "lh_beer_bottle_a_v1_bv01_01", "lh_beer_bottle_a_v1_bv01_02", "lh_beer_bottle_a_v1_bv01_03", "lh_beer_bottle_a_v1_bv01_04", "lh_beer_bottle_a_v1_bv01_05",
            "lh_beer_bottle_b_v1_bv01_00", "lh_beer_bottle_b_v1_bv01_01", "lh_beer_bottle_b_v1_bv01_02", "lh_beer_bottle_b_v1_bv01_03", "lh_beer_bottle_b_v1_bv01_04",
            "lh_beer_bottle_b_v1_bv01_05", "lh_beer_bottle_b_v1_bv01_06", "lh_beer_bottle_b_v1_bv01_07", "lh_beer_bottle_b_v1_bv01_08", "lh_beer_bottle_b_v1_bv01_09",
            "lh_beer_bottle_b_v1_bv01_12", "lh_beer_bottle_b_v1_bv01_14", "lh_beer_bottle_b_v1_bv01_15", "lh_beer_bottle_b_v1_bv01_16", "lh_beer_bottle_b_v1_bv01_17",
            "lh_beer_bottle_b_v1_bv01_18", "lh_beer_bottle_b_v1_bv01_19", "lh_beer_bottle_b_v1_bv01_20", "lh_beer_bottle_b_v1_bv01_21",
            "lh_beer_case_a_v1", "lh_beer_case_a_v1_bv01_00", "lh_beer_case_a_v1_bv01_01",
            "lh_beer_crate_a_v1_bv01_00", "lh_beer_crate_a_v1_bv01_01", "lh_beer_crate_a_v1_bv01_03", "lh_beer_crate_a_v1_bv01_04", "lh_beer_crate_a_v1_bv01_05",
            "lh_beer_crate_a_v1_bv01_06", "lh_beer_crate_a_v1_bv01_07", "lh_beer_crate_a_v1_bv01_08", "lh_beer_crate_a_v1_bv01_09", "lh_beer_crate_a_v1_bv01_10",
            "lh_beer_crate_b_bv01_00", "lh_beer_crate_b_bv01_01", "lh_beer_crate_b_bv01_02", "lh_beer_crate_b_bv01_03", "lh_beer_crate_b_bv01_04",
            "lh_beer_crate_b_bv01_05", "lh_beer_crate_b_bv01_06", "lh_beer_crate_b_bv01_07", "lh_beer_crate_b_bv01_08", "lh_beer_crate_b_bv01_09",
            "lh_beer_crate_b_bv01_10", "lh_beer_crate_b_bv01_11", "lh_beer_crate_b_bv01_12", "lh_beer_crate_b_bv01_13", "lh_beer_crate_b_bv01_14",
            "lh_beer_crate_b_bv01_15", "lh_beer_crate_b_bv01_16", "lh_beer_crate_b_bv01_17", "lh_beer_crate_b_bv01_18", "lh_beer_crate_b_bv01_19",
            "lh_beer_crate_b_bv01_20", "lh_beer_crate_b_bv01_21", "lh_beer_crate_b_bv01_22", "lh_beer_crate_b_bv01_23",
            "lh_beer_crate_b_v1_bv01_00", "lh_beer_crate_b_v1_bv01_01",
            "lh_beer_mug_a_v1_bv01_00", "lh_beer_mug_a_v1_bv01_01", "lh_beer_mug_a_v1_bv01_02", "lh_beer_mug_a_v1_bv01_03", "lh_beer_mug_a_v1_bv01_04",
            "lh_beer_mug_a_v1_bv01_05", "lh_beer_mug_a_v1_bv01_06", "lh_beer_mug_a_v1_bv01_07", "lh_beer_mug_a_v1_bv01_09",
            "lh_beer_mug_a_v1_bv01_10", "lh_beer_mug_a_v1_bv01_11", "lh_beer_mug_a_v1_bv01_12", "lh_beer_mug_a_v1_bv01_13", "lh_beer_mug_a_v1_bv01_14",
            "lh_beer_mug_a_v1_bv01_15", "lh_beer_mug_a_v1_bv01_16", "lh_beer_mug_a_v1_bv01_17", "lh_beer_mug_a_v1_bv01_18", "lh_beer_mug_a_v1_bv01_19",
            "lh_bench_a_v1_bv01_00", "lh_bench_a_v1_bv01_02", "lh_bench_a_v1_bv01_03", "lh_bench_a_v1_bv01_05", "lh_bench_a_v1_bv01_06", "lh_bench_a_v1_bv01_07",
            "lh_bench_a_v1_bv01_09", "lh_bench_a_v1_bv01_10", "lh_bench_a_v1_bv01_13", "lh_bench_a_v1_bv01_16", "lh_bench_a_v1_bv01_17", "lh_bench_a_v1_bv01_19",
            "lh_bench_a_v1_bv01_20", "lh_bench_a_v1_bv01_21", "lh_bench_a_v1_bv01_22", "lh_bench_a_v1_bv01_23", "lh_bench_a_v1_bv01_24", "lh_bench_a_v1_bv01_25",
            "lh_bench_a_v1_bv01_26", "lh_bench_a_v1_bv01_27", "lh_bench_a_v1_bv01_28", "lh_bench_a_v1_bv01_29",
            "lh_billiard_cabinet_a_v1_bv01_00", "lh_billiard_table_a_v2_00",
            "lh_book_box_a_v1_bv01_00", "lh_book_opened_b_v1_bv01_00", "lh_book_opened_b_v1_bv01_01", "lh_book_set_a_v1", "lh_book_set_b_v1",
            "lh_books_crate_a_v1_bv01_00", "lh_books_crate_a_v1_bv01_02", "lh_books_crate_a_v1_bv01_03", "lh_books_crate_a_v1_bv01_04", "lh_books_crate_a_v1_bv01_05", "lh_books_crate_a_v1_bv01_06",
            "lh_bookshelf_c_v1_bv01_00",
            "lh_bottle_spirits_e_v1_bv01_00", "lh_bottle_spirits_f_v1_bv01_00", "lh_bottle_spirits_g_v1_bv01_00", "lh_bottle_spirits_g_v1_bv01_01",
            "lh_bottle_whiskey_a_v1", "lh_bottle_whiskey_a_v1_bv01_00", "lh_bottle_whiskey_a_v1_bv01_01",
            "lh_bowl_soup_a_v1_bv01_00", "lh_bowl_soup_a_v1_bv01_01", "lh_bowl_soup_a_v1_bv01_02", "lh_bowl_soup_a_v1_bv01_03", "lh_bowl_soup_a_v1_bv01_04",
            "lh_box_pile_tarpaulin_a_v1_bv01_00", "lh_box_pile_tarpaulin_a_v1_bv01_01", "lh_box_pile_tarpaulin_a_v1_bv01_02", "lh_box_pile_tarpaulin_a_v1_bv01_03",
            "lh_bread_a_v1_bv01_00", "lh_bread_a_v1_bv01_01", "lh_bread_a_v1_bv01_02", "lh_bread_a_v1_bv01_03", "lh_bread_a_v1_bv01_04", "lh_bread_a_v1_bv01_05", "lh_bread_a_v1_bv01_06",
            "lh_briefcase_a_v1_bv01_00", "lh_broom_a_v1_bv01_00", "lh_broom_a_v1_bv01_01", "lh_cakebox_b_v1_bv01_00",
            "lh_car_creeper_a_v1_bv01_00", "lh_car_creeper_a_v1_bv01_01", "lh_car_creeper_a_v1_bv01_02",
            "lh_car_door_a_v1_bv_01", "lh_car_door_a_v1_bv_02", "lh_car_door_a_v1_bv_03", "lh_car_door_a_v1_bv_04", "lh_car_door_a_v1_bv_05", "lh_car_door_a_v1_bv_06", "lh_car_door_a_v1_bv_07",
            "lh_car_engine_a_v1_bv01_00",
            "lh_carpet_rolls_a_v1_bv01_00", "lh_carpet_rolls_a_v1_bv01_01", "lh_carpet_rolls_a_v1_bv01_02", "lh_carrot_slices_a_v1_bv01_00",
            "lh_ceiling_fan_a_v1_bv01_00", "lh_ceiling_fan_a_v1_bv01_01", "lh_ceiling_lamp_a_v1_01", "lh_ceiling_lamp_b_v1", "lh_ceiling_lamp_b_v1_bv01_00", "lh_celiling_fan_b_v1",
            "lh_cement_bag_a_v1_bv01_00", "lh_cement_bag_a_v1_bv01_02", "lh_cement_bag_a_v1_bv01_03", "lh_cement_bag_a_v1_bv01_04", "lh_cement_bag_a_v1_bv01_05",
            "lh_chair_d_v1", "lh_chair_e_v1", "lh_chair_f_v1", "lh_chair_f_v2_bv01_00", "lh_chair_f_v2_bv01_02", "lh_chair_f_v2_bv01_03",
            "lh_champagne_bottle_a_v1_bv01_00", "lh_champagne_bottle_a_v1_bv01_01", "lh_champagne_bottle_a_v1_bv01_02", "lh_champagne_bottle_a_v1_bv01_03",
            "lh_cigar_a_v1_bv01_00", "lh_cigar_box_a_v1", "lh_cigar_box_b_v1", "lh_cigar_box_closed_a_v1", "lh_cigar_box_full_a_v1", "lh_cigar_box_full_b_v1",
            "lh_cigar_pile_a_v1_bv01_00", "lh_cigarette_pack_a_v1_bv01_00", "lh_cigarette_pack_b_v1_bv01", "lh_cigarette_pack_b_v1_bv01_00",
            "lh_cigars_crate_a_v1_bv01_00", "lh_cigars_crate_a_v1_bv01_01", "lh_cigars_crate_a_v1_bv01_05", "lh_cigars_crate_a_v1_bv01_06",
            "lh_city_chimney_a_v1_00", "lh_city_chimney_a_v1_bv01_00", "lh_city_chimney_b_v1",
            "lh_city_door_b_static_v1_bv01_00", "lh_city_fence_2m_a_v1", "lh_city_lamp_wall_b_v1_bv01_00",
            "lh_city_paper_underaly_a_v1", "lh_city_paper_underaly_a_v1_bv01_01",
            "lh_city_poster_b_v1_bv01_00", "lh_city_poster_b_v1_bv01_01", "lh_city_poster_b_v1_bv01_02", "lh_city_poster_b_v1_bv01_03",
            "lh_city_poster_c_v1_bv01_00", "lh_city_poster_c_v1_bv01_01", "lh_city_poster_c_v1_bv01_02", "lh_city_poster_c_v1_bv01_03", "lh_city_poster_c_v1_bv01_04",
            "lh_city_poster_c_v1_bv01_05", "lh_city_poster_c_v1_bv01_06", "lh_city_poster_c_v1_bv01_07", "lh_city_poster_c_v1_bv01_08", "lh_city_poster_c_v1_bv01_09",
            "lh_city_poster_c_v1_bv01_10", "lh_city_poster_c_v1_bv01_11", "lh_city_poster_c_v1_bv01_15", "lh_city_poster_c_v1_bv01_16", "lh_city_poster_c_v1_bv01_17",
            "lh_city_poster_c_v1_bv01_18", "lh_city_poster_c_v1_bv01_19", "lh_city_poster_c_v1_bv01_20", "lh_city_poster_c_v1_bv01_23", "lh_city_poster_c_v1_bv01_25",
            "lh_city_poster_c_v1_bv01_26", "lh_city_poster_c_v1_bv01_27", "lh_city_poster_c_v1_bv01_28", "lh_city_poster_c_v1_bv01_29",
            "lh_city_poster_wanted_a_v1_00",
            "lh_city_poster_wanted_c_v1_00", "lh_city_poster_wanted_c_v1_01", "lh_city_poster_wanted_c_v1_02", "lh_city_poster_wanted_c_v1_03", "lh_city_poster_wanted_c_v1_05",
            "lh_city_poster_wanted_c_v1_06", "lh_city_poster_wanted_c_v1_07", "lh_city_poster_wanted_c_v1_08", "lh_city_poster_wanted_c_v1_09", "lh_city_poster_wanted_c_v1_10",
            "lh_city_poster_wanted_c_v1_11", "lh_city_poster_wanted_c_v1_12", "lh_city_poster_wanted_c_v1_13", "lh_city_poster_wanted_c_v1_14", "lh_city_poster_wanted_c_v1_15",
            "lh_city_poster_wanted_c_v1_16", "lh_city_poster_wanted_c_v1_17", "lh_city_poster_wanted_c_v1_18", "lh_city_poster_wanted_c_v1_19", "lh_city_poster_wanted_c_v1_20",
            "lh_city_poster_wanted_c_v1_22", "lh_city_poster_wanted_c_v1_23", "lh_city_poster_wanted_c_v1_24", "lh_city_poster_wanted_c_v1_25", "lh_city_poster_wanted_c_v1_26",
            "lh_city_poster_wanted_c_v1_28", "lh_city_poster_wanted_c_v1_29", "lh_city_poster_wanted_c_v1_30", "lh_city_poster_wanted_c_v1_31",
            "lh_city_roof_chimney_a_v1_bv01_00", "lh_city_roof_chimney_a_v1_bv01_01", "lh_city_roof_chimney_a_v1_bv01_02", "lh_city_roof_chimney_a_v1_bv01_03", "lh_city_roof_chimney_a_v1_bv01_04",
            "lh_city_roof_chimney_a_v1_bv01_09", "lh_city_roof_chimney_a_v1_bv01_23", "lh_city_roof_chimney_a_v1_bv01_24", "lh_city_roof_chimney_a_v1_bv01_25", "lh_city_roof_chimney_a_v1_bv01_26", "lh_city_roof_chimney_a_v1_bv01_27",
            "lh_city_roof_chimney_c_v1", "lh_city_street_sewer_a_v1", "lh_city_sunshade_a_v1_bv01_04",
            "lh_cleaver_a_v1_bv01_00", "lh_clipboard_a_v1", "lh_clipboard_a_v1_bv01_00", "lh_clipboard_a_v1_bv01_01", "lh_clipboard_a_v1_bv01_02", "lh_clipboard_a_v1_bv01_03", "lh_clipboard_a_v1_bv01_04",
            "lh_coin_a_v1_bv01_00", "lh_coin_a_v1_bv01_01", "lh_corn_a_v1_bv01_00", "lh_corn_a_v1_bv01_01", "lh_couch_c_v1",
            "lh_cup_a_v1_bv01_00", "lh_cup_a_v1_bv01_01", "lh_cup_a_v1_bv01_02", "lh_cup_a_v1_bv01_03", "lh_cup_a_v1_bv01_04",
            "lh_cup_a_v1_bv01_05", "lh_cup_a_v1_bv01_06", "lh_cup_a_v1_bv01_07", "lh_cup_a_v1_bv01_08", "lh_cup_a_v1_bv01_09",
            "lh_cup_a_v1_bv01_13", "lh_cup_a_v1_bv01_14", "lh_cup_a_v1_bv01_15", "lh_cup_a_v1_bv01_16",
            "lh_double_oil_barrel_coverable_a_v2", "lh_double_table_small_b_v1_bv01_00", "lh_double_table_small_b_v1_bv01_01",
            "lh_drill_press_a_v1_bv01_00", "lh_drill_press_a_v1_bv01_01",
            "lh_fan_a_v1_bv01_00", "lh_fan_a_v1_bv01_01", "lh_fan_a_v1_bv01_02", "lh_fan_a_v1_bv01_03", "lh_fence_a_v1",
            "lh_flatware_fork_a_v1_bv01_00", "lh_flatware_knife_a_v1_bv01_00",
            "lh_flour_sack_stack_a_v1_bv01_00", "lh_flour_sack_stack_a_v1_bv01_01", "lh_flour_sack_stack_a_v1_bv01_02",
            "lh_folded_note_a_v1", "lh_folded_note_a_v1_bv01", "lh_folded_note_a_v1_bv01_01", "lh_folded_note_a_v1_bv02",
            "lh_garbage_a_v1", "lh_garbage_bin_a_v1", "lh_garbage_bin_b_v1", "lh_garbage_bin_d_v1", "lh_garbage_c_v1", "lh_garbage_e_v1", "lh_garbage_f_v1",
            "lh_garbage_food_pile_a_v1", "lh_garbage_glass_a_v1", "lh_garbage_glass_b_v1", "lh_garbage_painter_e_v1",
            "lh_garbage_paper_box_a_v1", "lh_garbage_paper_box_a_v1_bv01_11", "lh_garbage_paper_box_b_v1", "lh_garbage_paper_box_b_v1_bv01_14", "lh_garbage_shed_a_v1",
            "lh_gate_a_closed_unbreakable_v1_bv01_00",
            "lh_handcart_a_v1", "lh_handcart_a_v1_bv01_00", "lh_handcart_b_v1", "lh_handcart_b_v1_bv01_01", "lh_handcart_b_v1_bv01_02",
            "lh_hanging_electrical_wire_a_v1", "lh_hanging_electrical_wire_b_v1_bv01_00", "lh_hanging_electrical_wire_b_v1_bv01_01", "lh_hanging_lamp_b_v2_bv01_00",
            "lh_hat_a_v1_bv01_00", "lh_hat_rack_a_v1_bv01_00", "lh_hat_rack_a_v1_bv01_01",
            "lh_hotdog_cart_a_v1", "lh_hotdog_tongs_a_v1_bv01_00", "lh_hotdog_tongs_a_v1_bv01_01", "lh_hydrant_a_v1",
            "lh_kitchen_knife_a_v1_bv01_00", "lh_kitchen_knife_a_v1_bv01_01", "lh_kitchen_knife_a_v1_bv01_02", "lh_kitchen_knife_a_v1_bv01_03", "lh_kitchen_knife_a_v1_bv01_04",
            "lh_kitchen_ladle_a_v1_bv01_00", "lh_kitchen_ladle_a_v1_bv01_01", "lh_kitchen_ladle_a_v1_bv01_02", "lh_kitchen_ladle_a_v1_bv01_03", "lh_kitchen_ladle_a_v1_bv01_04",
            "lh_kitchen_ladle_a_v1_bv01_05", "lh_kitchen_ladle_a_v1_bv01_06", "lh_kitchen_ladle_a_v1_bv01_07",
            "lh_kitchen_plate_a_v1_bv01_00", "lh_kitchen_plate_a_v1_bv01_01", "lh_kitchen_plate_a_v1_bv01_02", "lh_kitchen_plate_a_v1_bv01_03", "lh_kitchen_plate_a_v1_bv01_04", "lh_kitchen_plate_a_v1_bv01_05", "lh_kitchen_plate_a_v1_bv01_06",
            "lh_kitchen_pot_a_v1_bv01_00", "lh_kitchen_pot_a_v1_bv01_01", "lh_kitchen_pot_a_v1_bv01_02", "lh_kitchen_pot_b_v1_bv01_00", "lh_kitchen_pot_b_v1_bv01_01",
            "lh_kitchen_sink_a_v1_bv01_00", "lh_kitchen_utensils_a_v1_bv01_00", "lh_lamp_bulb_wire_a_v1_bv01_00",
            "lh_leaning_pallets_a_v1", "lh_leaning_pallets_a_v1_bv01_00", "lh_lemon_a_v1_bv01_00",
            "lh_lifter_tool_a_v1_bv01_00", "lh_lifter_tool_a_v1_bv01_01", "lh_lifter_tool_a_v1_bv01_02",
            "lh_loc_abandoned_diner_barrel_oil_a_v1_bv01_00", "lh_loc_salieris_bar_booth_a_v1_bv01_00", "lh_loc_salieris_bar_gate_a_v1_bv01_00", "lh_locker_a_v1",
            "lh_lunch_bag_a_v1_bv01_00", "lh_lunch_bag_a_v1_bv01_01", "lh_lunch_bag_a_v1_bv01_02", "lh_lunch_bag_a_v1_bv01_03", "lh_lunch_bag_a_v1_bv01_04",
            "lh_luxury_chair_b_v1_bv01_00", "lh_luxury_desk_a_v1_bv01_00", "lh_mailbox_b_v1",
            "lh_metal_barrel_a_v1_bv01_00", "lh_metal_barrel_a_v1_bv01_10",
            "lh_metal_bucket_a_v1_bv01_00", "lh_metal_bucket_a_v1_bv01_01", "lh_metal_bucket_a_v1_bv01_02", "lh_metal_bucket_a_v1_bv01_03", "lh_metal_bucket_a_v1_bv01_04", "lh_metal_bucket_a_v1_bv01_05",
            "lh_metal_shelf_b_v1", "lh_metalbox_junk_a_v1",
            "lh_milk_bottle_a_v1_bv01_00", "lh_milk_bottle_a_v1_bv01_01", "lh_milk_crate_a_v1_bv01_00", "lh_milk_crate_a_v1_bv01_01", "lh_milk_crate_a_v1_bv01_02",
            "lh_milkbarrel_a_v1_bv01_00", "lh_milkbarrel_a_v1_bv01_01", "lh_milkbarrel_a_v1_bv01_02", "lh_milkbarrel_a_v1_bv01_03", "lh_milkbarrel_a_v1_bv01_04",
            "lh_milkbarrel_a_v1_bv01_05", "lh_milkbarrel_a_v1_bv01_06", "lh_milkbarrel_a_v1_bv01_07", "lh_milkbarrel_a_v1_bv01_08", "lh_milkbarrel_a_v1_bv01_09", "lh_milkbarrel_a_v1_bv01_18",
            "lh_money_stack_a_v1_bv01_01", "lh_money_stack_a_v1_bv01_02", "lh_money_stack_a_v1_bv01_03", "lh_money_stack_a_v1_bv01_04", "lh_money_stack_a_v1_bv01_05",
            "lh_money_stack_a_v1_bv01_06", "lh_money_stack_a_v1_bv01_07", "lh_money_stack_a_v1_bv01_08", "lh_money_stack_a_v1_bv01_09", "lh_money_stack_a_v1_bv01_10",
            "lh_money_stack_a_v1_bv01_11", "lh_money_stack_a_v1_bv01_12", "lh_money_stack_a_v1_bv01_13", "lh_money_stack_a_v1_bv01_14",
            "lh_newspaper_a_v1_bv01_00", "lh_newspaper_a_v1_bv01_01", "lh_newspaper_a_v1_bv01_02", "lh_newspaper_a_v1_bv01_03", "lh_newspaper_a_v1_bv01_04",
            "lh_newspaper_a_v1_bv01_05", "lh_newspaper_a_v1_bv01_06", "lh_newspaper_a_v1_bv01_07", "lh_newspaper_a_v1_bv01_08", "lh_newspaper_a_v1_bv01_09",
            "lh_newspaper_a_v1_bv01_10", "lh_newspaper_a_v1_bv01_11", "lh_newspaper_a_v1_bv01_12", "lh_newspaper_a_v1_bv01_13", "lh_newspaper_a_v1_bv01_14",
            "lh_newspaper_b_v1_bv01_00",
            "lh_newspaper_folded_a_v1_bv01_00", "lh_newspaper_folded_a_v1_bv01_01", "lh_newspaper_folded_a_v1_bv01_02", "lh_newspaper_folded_a_v1_bv01_03", "lh_newspaper_folded_a_v1_bv01_04",
            "lh_newspaper_pack_a_v1_bv01_00", "lh_newspaper_pack_a_v1_bv01_01", "lh_newspaper_pack_a_v1_bv01_02", "lh_newspaper_pack_a_v1_bv01_03",
            "lh_nostalgia_a_bv01_00", "lh_nostalgia_b_bv01_00",
            "lh_note_newspaper_folded_Bobby", "lh_note_newspaper_folded_Crash", "lh_note_newspaper_folded_Prohibition", "lh_note_newspaper_folded_Race", "lh_note_newspaper_folded_Turnbull",
            "lh_notepad_a_v1_bv01_00",
            "lh_notepad_small_a_v1_bv01_01", "lh_notepad_small_a_v1_bv01_02", "lh_notepad_small_a_v1_bv01_03", "lh_notepad_small_a_v1_bv01_04", "lh_notepad_small_a_v1_bv01_05",
            "lh_notepad_small_a_v1_bv01_06", "lh_notepad_small_a_v1_bv01_07", "lh_notepad_small_a_v1_bv01_08", "lh_notepad_small_a_v1_bv01_09", "lh_notepad_small_a_v1_bv01_10",
            "lh_office_wall_board_a_v1", "lh_oil_can_a_v1", "lh_oil_can_a_v1_bv01_00", "lh_oil_can_a_v1_bv01_01", "lh_oil_can_b_v1",
            "lh_old_apple_a_v1_bv01_00", "lh_old_apple_a_v1_bv01_01", "lh_old_apple_a_v1_bv01_02", "lh_old_apple_a_v1_bv01_03", "lh_old_apple_a_v1_bv01_04",
            "lh_orange_a_v1_bv01_00", "lh_orange_a_v1_bv01_01", "lh_orange_a_v1_bv01_02", "lh_orange_a_v1_bv01_03", "lh_orange_a_v1_bv01_04",
            "lh_outdoor_lamp_a_v2_bv01_00", "lh_outdoor_lamp_d_v1_bv01_00", "lh_outdoor_table_a_v1", "lh_outdoor_work_table_a_v1", "lh_outdoor_work_table_b_v1",
            "lh_paint_bucket_a_v1_bv01_00", "lh_paint_bucket_a_v1_bv01_01", "lh_paint_bucket_a_v1_bv01_02", "lh_paint_bucket_a_v1_bv01_03", "lh_paint_bucket_a_v1_bv01_04",
            "lh_paint_bucket_a_v1_bv01_05", "lh_paint_bucket_a_v1_bv01_06", "lh_paint_bucket_a_v1_bv01_07", "lh_paint_bucket_a_v1_bv01_08", "lh_paint_bucket_a_v1_bv01_09",
            "lh_paint_bucket_a_v1_bv01_10", "lh_paint_bucket_a_v1_bv01_11", "lh_paint_bucket_a_v1_bv01_12", "lh_paint_bucket_a_v1_bv01_13",
            "lh_paint_can_a_v1_bv01_00", "lh_paint_can_a_v1_bv01_01", "lh_paintbrush_a_v1_bv01_00", "lh_paintbrush_a_v1_bv01_01",
            "lh_pallet_pack_a_kit_v1_bv01", "lh_pallet_pack_c_kit_v1_bv01", "lh_pallet_pack_c_kit_v1_bv01_00",
            "lh_pallets_a_v1", "lh_pallets_a_v1_bv01_00", "lh_pallets_a_v1_bv01_01", "lh_pallets_a_v1_bv01_02",
            "lh_pallets_a_v2_bv01_00", "lh_pallets_a_v2_bv01_01", "lh_pallets_c_v1", "lh_pallets_c_v1_bv01_04",
            "lh_paperbox_a_v1", "lh_paperbox_a_v1_bv01_11", "lh_paperbox_b_v1", "lh_paperbox_b_v1_bv01_00", "lh_paperbox_c_v1", "lh_paperbox_e_v1",
            "lh_pear_a_v1_bv01_00", "lh_pear_a_v1_bv01_01",
            "lh_pen_a_v1_bv01_00", "lh_pen_a_v1_bv01_01", "lh_pen_a_v1_bv01_02", "lh_pen_a_v1_bv01_03", "lh_pen_a_v1_bv01_04", "lh_pen_a_v1_bv01_05",
            "lh_pepper_a_v1_bv01_00", "lh_pepper_a_v1_bv01_01", "lh_pepper_a_v1_bv01_02", "lh_pepper_a_v1_bv01_03",
            "lh_petrol_lamp_a_v1_bv01_00", "lh_petrol_lamp_a_v1_bv01_01", "lh_petrol_lamp_a_v1_bv01_02", "lh_petrol_lamp_a_v1_bv01_03",
            "lh_pickup_bm_1944_03_bv01", "lh_pickup_cc_10_felice_peppone_bv01", "lh_pickup_dd_1944_12_bv01", "lh_pickup_fox_statue_a_v2_bv01_00", "lh_pickup_sss_1940_07_bv01",
            "lh_picnic_table_bench_a_v1",
            "lh_picture_frame_b_v1_bv01_01", "lh_picture_frame_b_v1_bv01_02", "lh_picture_frame_b_v1_bv01_03", "lh_picture_frame_b_v1_bv01_04", "lh_picture_frame_b_v1_bv01_05",
            "lh_picture_frame_c_v1_bv01_00", "lh_picture_frame_c_v1_bv01_01", "lh_picture_frame_c_v1_bv01_02",
            "lh_picture_frame_d_v1_bv01_00", "lh_picture_frame_d_v1_bv01_02", "lh_picture_frame_d_v1_bv01_03", "lh_picture_frame_d_v1_bv01_04",
            "lh_picture_frame_g_v1_bv01_00", "lh_picture_frame_g_v1_bv01_01",
            "lh_picture_frame_h_v1_bv01_00", "lh_picture_frame_h_v1_bv01_01", "lh_picture_frame_h_v1_bv01_02", "lh_picture_frame_h_v1_bv01_03", "lh_picture_frame_h_v1_bv01_04", "lh_picture_frame_h_v1_bv01_05", "lh_picture_frame_h_v1_bv01_06",
            "lh_picture_frame_i_v1_bv01_00", "lh_picture_frame_i_v1_bv01_01", "lh_picture_frame_i_v1_bv01_02", "lh_picture_frame_i_v1_bv01_03",
            "lh_picture_frame_p_v1",
            "lh_pizza_plate_a_v1_bv01_00", "lh_pizza_plate_b_v1_bv01_00",
            "lh_plate_a_v1_bv01_00", "lh_plate_a_v1_bv01_01", "lh_plate_a_v1_bv01_02", "lh_plate_a_v1_bv01_03", "lh_plate_a_v1_bv01_04",
            "lh_plate_a_v1_bv01_05", "lh_plate_a_v1_bv01_06", "lh_plate_a_v1_bv01_07", "lh_plate_a_v1_bv01_08",
            "lh_playing_cards_a_v1_bv01_00", "lh_playing_cards_a_v2_bv01_00",
            "lh_playing_cards_a_v4_bv01_00", "lh_playing_cards_a_v4_bv01_01", "lh_playing_cards_a_v4_bv01_02", "lh_playing_cards_a_v4_bv01_03",
            "lh_playing_cards_deck_a_v1_bv01_00", "lh_playing_cards_deck_a_v1_bv01_01", "lh_playing_cards_deck_a_v1_bv01_02", "lh_playing_cards_deck_a_v1_bv01_03",
            "lh_pliers_a_v1_bv01_00", "lh_pliers_a_v1_bv01_01", "lh_pliers_b_v1_bv01_00", "lh_pliers_b_v1_bv01_01",
            "lh_postcard_i_v1_bv01_00", "lh_postcard_i_v1_bv01_01", "lh_postcard_j_v1_bv01_00", "lh_postcard_j_v1_bv01_01",
            "lh_punching_bag_v1_bv01_00",
            "lh_radio_c_v1_00", "lh_radio_c_v1_bv01_00", "lh_radio_c_v1_bv01_01", "lh_radio_c_v1_bv01_02",
            "lh_rag_a_v1_bv01_00", "lh_rag_a_v1_bv01_01", "lh_rag_a_v1_bv01_02", "lh_rag_b_v1_bv01_02", "lh_rag_b_v1_bv01_05",
            "lh_rake_a_v1_bv01_00",
            "lh_rope_pile_a_v1_bv01_00", "lh_rope_pile_b_v1_bv01_00", "lh_rope_pile_b_v1_bv01_01", "lh_rope_pile_b_v1_bv01_02", "lh_rope_pile_b_v1_bv01_03",
            "lh_rope_pile_b_v1_bv01_04", "lh_rope_pile_b_v1_bv01_05", "lh_rope_pile_b_v1_bv01_06", "lh_rope_pile_b_v1_bv01_07",
            "lh_sack_a_v1_bv01_01", "lh_sack_a_v1_bv01_02", "lh_sack_a_v1_bv01_03", "lh_sack_a_v1_bv01_06", "lh_sack_a_v1_bv01_07",
            "lh_sack_b_v1_06", "lh_sack_b_v1_09", "lh_sack_b_v1_bv01_00", "lh_sack_b_v1_bv01_02", "lh_sack_b_v1_bv01_03", "lh_sack_b_v1_bv01_04",
            "lh_sack_c_v1_bv01_00", "lh_sack_c_v1_bv01_01", "lh_sack_c_v1_bv01_02",
            "lh_sandwhich_a_v1_bv01_00", "lh_sandwhich_a_v1_bv01_01", "lh_sandwhich_a_v1_bv01_02",
            "lh_sawhorse_wood_desk_a_v1", "lh_sawhorse_wood_plank_a_v1", "lh_sawhorse_wood_plank_c_v1", "lh_screwdriver_a_v1_bv01_00",
            "lh_shelf_prop_bag_a_v1_bv01_00", "lh_shelf_prop_bag_b_v1_bv01_00",
            "lh_shelf_prop_bag_c_v1_bv01_00", "lh_shelf_prop_bag_c_v1_bv01_01", "lh_shelf_prop_bag_c_v1_bv01_02",
            "lh_shelf_prop_bag_d_v1_bv01_00", "lh_shelf_prop_bag_d_v1_bv01_01",
            "lh_shelf_prop_bowl_a_v1_bv01_00", "lh_shelf_prop_bowl_a_v1_bv01_01",
            "lh_shelf_prop_box_a_v1_bv01_00", "lh_shelf_prop_box_a_v1_bv01_01",
            "lh_shelf_prop_box_b_v1_bv01_00", "lh_shelf_prop_box_b_v1_bv01_01", "lh_shelf_prop_box_b_v1_bv01_02", "lh_shelf_prop_box_b_v1_bv01_03", "lh_shelf_prop_box_b_v1_bv01_04", "lh_shelf_prop_box_b_v1_bv01_05", "lh_shelf_prop_box_b_v1_bv01_06",
            "lh_shelf_prop_box_c_v1_bv01_00", "lh_shelf_prop_box_c_v1_bv01_01", "lh_shelf_prop_box_c_v1_bv01_02", "lh_shelf_prop_box_c_v1_bv01_03", "lh_shelf_prop_box_c_v1_bv01_04", "lh_shelf_prop_box_c_v1_bv01_05", "lh_shelf_prop_box_c_v1_bv01_06", "lh_shelf_prop_box_c_v1_bv01_07",
            "lh_shelf_prop_box_d_v1_bv01_00", "lh_shelf_prop_box_d_v1_bv01_01", "lh_shelf_prop_box_d_v1_bv01_02", "lh_shelf_prop_box_d_v1_bv01_03", "lh_shelf_prop_box_d_v1_bv01_04",
            "lh_shelf_prop_box_d_v1_bv01_05", "lh_shelf_prop_box_d_v1_bv01_06", "lh_shelf_prop_box_d_v1_bv01_07", "lh_shelf_prop_box_d_v1_bv01_08", "lh_shelf_prop_box_d_v1_bv01_09", "lh_shelf_prop_box_d_v1_bv01_10",
            "lh_shelf_prop_box_e_v1_bv01_00", "lh_shelf_prop_box_e_v1_bv01_01", "lh_shelf_prop_box_e_v1_bv01_02", "lh_shelf_prop_box_e_v1_bv01_03",
            "lh_shelf_prop_box_f_v1_09", "lh_shelf_prop_box_f_v1_bv01_00", "lh_shelf_prop_box_f_v1_bv01_01", "lh_shelf_prop_box_f_v1_bv01_02", "lh_shelf_prop_box_f_v1_bv01_03", "lh_shelf_prop_box_f_v1_bv01_10",
            "lh_shelf_prop_box_h_v1_bv01_00", "lh_shelf_prop_box_i_v1_bv01_00", "lh_shelf_prop_box_i_v1_bv01_01",
            "lh_shelf_prop_box_k_v1_bv01_02", "lh_shelf_prop_box_k_v1_bv01_03", "lh_shelf_prop_box_k_v1_bv01_04", "lh_shelf_prop_box_k_v1_bv01_05", "lh_shelf_prop_box_k_v1_bv01_06", "lh_shelf_prop_box_k_v1_bv01_09", "lh_shelf_prop_box_k_v1_bv01_10",
            "lh_shelf_prop_box_l_v1_bv01_00", "lh_shelf_prop_box_l_v1_bv01_01", "lh_shelf_prop_box_l_v1_bv01_02",
            "lh_shelf_prop_box_l_v1_bv01_12", "lh_shelf_prop_box_l_v1_bv01_13", "lh_shelf_prop_box_l_v1_bv01_14", "lh_shelf_prop_box_l_v1_bv01_15", "lh_shelf_prop_box_l_v1_bv01_16", "lh_shelf_prop_box_l_v1_bv01_17",
            "lh_shelf_prop_box_n_v1", "lh_shelf_prop_box_o_v1_bv01_00", "lh_shelf_prop_box_o_v1_bv01_01", "lh_shelf_prop_box_o_v1_bv01_02",
            "lh_shelf_prop_can_a_v1_bv01_00", "lh_shelf_prop_can_a_v1_bv01_01", "lh_shelf_prop_can_a_v1_bv01_02", "lh_shelf_prop_can_a_v1_bv01_03",
            "lh_shelf_prop_can_b_v1_bv01_00", "lh_shelf_prop_can_b_v1_bv01_01", "lh_shelf_prop_can_b_v1_bv01_02", "lh_shelf_prop_can_b_v1_bv01_03",
            "lh_shelf_prop_can_c_v1_bv01_00", "lh_shelf_prop_can_c_v1_bv01_01", "lh_shelf_prop_can_c_v1_bv01_02", "lh_shelf_prop_can_c_v1_bv01_03", "lh_shelf_prop_can_c_v1_bv01_04",
            "lh_shelf_prop_can_d_v1_bv01_00", "lh_shelf_prop_can_d_v1_bv01_01", "lh_shelf_prop_can_e_v1_bv01_00", "lh_shelf_prop_can_f_v1_bv01_00", "lh_shelf_prop_can_f_v1_bv01_01",
            "lh_shelf_prop_can_l_v1_bv01_00", "lh_shelf_prop_can_l_v1_bv01_01", "lh_shelf_prop_can_l_v1_bv01_02", "lh_shelf_prop_can_l_v1_bv01_03", "lh_shelf_prop_can_l_v1_bv01_04", "lh_shelf_prop_can_l_v1_bv01_05", "lh_shelf_prop_can_l_v1_bv01_06", "lh_shelf_prop_can_l_v1_bv01_07",
            "lh_shelf_prop_container_a_v1_bv01_00", "lh_shelf_prop_container_a_v1_bv01_01", "lh_shelf_prop_container_a_v1_bv01_02", "lh_shelf_prop_container_a_v1_bv01_03", "lh_shelf_prop_container_a_v1_bv01_04", "lh_shelf_prop_container_a_v1_bv01_05", "lh_shelf_prop_container_a_v1_bv01_06",
            "lh_shelf_prop_part_a_v1_bv01_00", "lh_shelf_prop_part_a_v1_bv01_01", "lh_shelf_prop_part_a_v1_bv01_02", "lh_shelf_prop_part_a_v1_bv01_03", "lh_shelf_prop_part_a_v1_bv01_04",
            "lh_shelf_prop_part_a_v1_bv01_05", "lh_shelf_prop_part_a_v1_bv01_06", "lh_shelf_prop_part_a_v1_bv01_07", "lh_shelf_prop_part_a_v1_bv01_08", "lh_shelf_prop_part_a_v1_bv01_09",
            "lh_shelf_prop_part_a_v1_bv01_10", "lh_shelf_prop_part_a_v1_bv01_11", "lh_shelf_prop_part_a_v1_bv01_12", "lh_shelf_prop_part_a_v1_bv01_13", "lh_shelf_prop_part_a_v1_bv01_14",
            "lh_shelf_prop_part_a_v1_bv01_15", "lh_shelf_prop_part_a_v1_bv01_16", "lh_shelf_prop_part_a_v1_bv01_17", "lh_shelf_prop_part_a_v1_bv01_18", "lh_shelf_prop_part_a_v1_bv01_19",
            "lh_shelf_prop_part_a_v1_bv01_20", "lh_shelf_prop_part_a_v1_bv01_21", "lh_shelf_prop_part_a_v1_bv01_22", "lh_shelf_prop_part_a_v1_bv01_23", "lh_shelf_prop_part_a_v1_bv01_24",
            "lh_shelf_prop_part_a_v1_bv01_25", "lh_shelf_prop_part_a_v1_bv01_26", "lh_shelf_prop_part_a_v1_bv01_27", "lh_shelf_prop_part_a_v1_bv01_28",
            "lh_shelf_prop_part_d_v1_bv01_00", "lh_shelf_prop_part_d_v1_bv01_01", "lh_shelf_prop_part_d_v1_bv01_03", "lh_shelf_prop_part_e_v1_bv01_00",
            "lh_shelf_prop_tool_d_v1_bv01_00", "lh_shelf_prop_tool_k_v1_bv01_01", "lh_shelf_prop_tool_k_v1_bv01_02",
            "lh_shelf_small_a_v1_bv01_00", "lh_shelf_small_a_v1_bv01_01", "lh_shelf_small_a_v1_bv01_02", "lh_shelf_small_a_v1_bv01_03", "lh_shelf_small_a_v1_bv01_04",
            "lh_shelf_tires_a_v1", "lh_shoe_polish_brush_a_v1_bv01_00", "lh_shoe_polish_rag_a_v1_bv01_00", "lh_shoe_shine_table_set_a_v1",
            "lh_shop_display_case_bakery_a_v1_bv01_00", "lh_shop_display_case_bakery_b_v1_bv01_00", "lh_shopper_basket_a_v1_bv01_01",
            "lh_shovel_a_v1_bv01_00", "lh_shovel_a_v1_bv01_01",
            "lh_sign_a_v1", "lh_sign_c_v1_bv01_00", "lh_sign_d_v1_bv01_00", "lh_sign_e_v1_bv01_00", "lh_sign_medium_a_v1", "lh_sign_small_b_v1",
            "lh_station_bench_grinder_a_v1_bv01_00", "lh_station_bench_grinder_a_v1_bv01_01", "lh_station_bench_grinder_a_v1_bv01_02", "lh_station_bench_grinder_a_v1_bv01_03", "lh_station_bench_grinder_a_v1_bv01_04", "lh_station_bench_grinder_a_v1_bv01_05",
            "lh_streetclock_a_v1", "lh_streetlamp_b_v1_bv01_00", "lh_streetlamp_d_v1_bv01_00", "lh_streetlamp_e_v1_bv01_02",
            "lh_suitcase_a_v1_bv01_00", "lh_table_napkin_a_v1_bv01_00", "lh_table_note_holder_a_v1_bv01_00", "lh_table_phone_a_v1",
            "lh_tablecloth_d_v1_bv01_01", "lh_tablecloth_d_v1_bv01_02", "lh_tablecloth_d_v1_bv01_04", "lh_tablecloth_d_v1_bv01_05",
            "lh_target_range_a_v1_bv01_00", "lh_telephonebox_a_v1_bv01_00",
            "lh_tire_a_kit_v1_00", "lh_tire_a_v1", "lh_tire_a_v1_bv01_00", "lh_tire_a_v1_bv01_01", "lh_tire_a_v1_bv01_02", "lh_tire_pile_a_v1_bv01_01",
            "lh_tool_pinboard_a_v1_bv01_00", "lh_tool_pinboard_a_v1_bv01_01", "lh_tool_pinboard_a_v1_bv01_02",
            "lh_towel_a_v1_bv01_00", "lh_towel_a_v1_bv01_01", "lh_towel_a_v1_bv01_02", "lh_towel_a_v1_bv01_03", "lh_towel_a_v1_bv01_04",
            "lh_towel_a_v1_bv01_05", "lh_towel_a_v1_bv01_06", "lh_towel_a_v1_bv01_07", "lh_towel_a_v1_bv01_08", "lh_towel_a_v1_bv01_09",
            "lh_towel_a_v1_bv01_10", "lh_towel_a_v1_bv01_11", "lh_towel_a_v1_bv01_12",
            "lh_traffic_sign_c_v1_bv01_00", "lh_traffic_sign_c_v1_bv01_01", "lh_traffic_sign_c_v1_bv01_02", "lh_traffic_sign_c_v2_bv01_00", "lh_traffic_sign_c_v2_bv01_01",
            "lh_trashbin_a_v1", "lh_vi_kit_handcart_barrel_a_v1_bv01", "lh_violin_case_trench_case_a_v1_bv01_00",
            "lh_wall_fastener_a_v1_bv01_01", "lh_wall_fastener_a_v1_bv01_02", "lh_wall_fastener_a_v1_bv01_03", "lh_wall_fastener_a_v1_bv01_04", "lh_wall_fastener_a_v1_bv01_05",
            "lh_wall_fastener_a_v1_bv01_06", "lh_wall_fastener_a_v1_bv01_07", "lh_wall_fastener_a_v1_bv01_08", "lh_wall_fastener_a_v1_bv01_09",
            "lh_wall_fastener_a_v2_bv01_00", "lh_wall_fastener_a_v2_bv01_01", "lh_wall_fastener_a_v2_bv01_02", "lh_wall_fastener_a_v2_bv01_03", "lh_wall_fastener_a_v2_bv01_05",
            "lh_wall_fastener_a_v2_bv01_06", "lh_wall_fastener_a_v2_bv01_07", "lh_wall_fastener_a_v2_bv01_08", "lh_wall_fastener_a_v2_bv01_09", "lh_wall_fastener_a_v2_bv01_10",
            "lh_wall_lamp_a_v1_bv01_03", "lh_wall_lamp_b_v1_bv01_06",
            "lh_wall_painting_set_a_v1", "lh_wall_painting_small_a_v1", "lh_wall_shelf_b_v1_bv01_00", "lh_wall_shelf_b_v1_bv01_01", "lh_wall_shelf_c_v1",
            "lh_washboard_a_v1_bv01_00", "lh_washboard_a_v1_bv01_01", "lh_water_trough_a_v1_bv01_00", "lh_water_trough_a_v1_bv01_01",
            "lh_whiskey_glass_a_v1_bv01_00", "lh_whiskey_glass_a_v1_bv01_01", "lh_whiskey_glass_a_v1_bv01_02", "lh_whiskey_glass_a_v1_bv01_03", "lh_whiskey_glass_a_v1_bv01_04",
            "lh_whiskey_glass_a_v1_bv01_06", "lh_whiskey_glass_a_v1_bv01_07", "lh_whiskey_glass_a_v1_bv01_08", "lh_whiskey_glass_a_v1_bv01_09", "lh_whiskey_glass_a_v1_bv01_10", "lh_whiskey_glass_a_v1_bv01_11",
            "lh_wine_bottle_pack_a_v1", "lh_wine_bottle_pack_a_v1_bv01_00", "lh_wine_bottle_pack_a_v1_bv01_01", "lh_wine_bottle_pack_a_v1_bv01_02", "lh_wine_bottle_pack_a_v1_bv01_03",
            "lh_wine_bottle_pack_a_v1_bv01_04", "lh_wine_bottle_pack_a_v1_bv01_05", "lh_wine_bottle_pack_a_v1_bv01_06", "lh_wine_bottle_pack_a_v1_bv01_07",
            "lh_wine_glass_a_v1_bv01_01",
            "lh_wine_glass_a_v2_bv01_24", "lh_wine_glass_a_v2_bv01_25", "lh_wine_glass_a_v2_bv01_26", "lh_wine_glass_a_v2_bv01_27", "lh_wine_glass_a_v2_bv01_28",
            "lh_wine_glass_a_v2_bv01_29", "lh_wine_glass_a_v2_bv01_30", "lh_wine_glass_a_v2_bv01_31", "lh_wine_glass_a_v2_bv01_32",
            "lh_wood_plank_a_v1_bv01_01", "lh_wooden_advert_a_v1", "lh_wooden_barrier_a_v2_bv01_00",
            "lh_wooden_box_a_2x1_bv01", "lh_wooden_box_a_2x1_bv01_00",
            "lh_wooden_box_a_v1_bv01_00", "lh_wooden_box_a_v1_bv01_01", "lh_wooden_box_a_v1_bv01_02", "lh_wooden_box_a_v1_bv01_03", "lh_wooden_box_a_v1_bv01_04",
            "lh_wooden_box_a_v1_bv01_06", "lh_wooden_box_a_v1_bv01_07", "lh_wooden_box_a_v1_bv01_08", "lh_wooden_box_a_v1_bv01_14",
            "lh_wooden_box_b_v1_bv01_00", "lh_wooden_box_b_v1_bv01_01", "lh_wooden_box_b_v1_bv01_02", "lh_wooden_box_b_v1_bv01_03",
            "lh_wooden_box_c_undestroyable_v1_bv01_01", "lh_wooden_box_group_a_v1_bv01_00", "lh_wooden_box_group_b_v1_bv01", "lh_wooden_box_small_a_v1",
            "lh_wooden_boxes_undestroyable_a_v2", "lh_wooden_cabinet_e_v1", "lh_wooden_cabinet_f_v1", "lh_wooden_cabinet_i_v1_bv01_00", "lh_wooden_cabinet_l_v1_bv01_00", "lh_wooden_cage_a_v1",
            "lh_wooden_chair_a_v1_bv01_00", "lh_wooden_chair_a_v1_bv01_01", "lh_wooden_chair_a_v1_bv01_02", "lh_wooden_chair_a_v1_bv01_03", "lh_wooden_chair_a_v1_bv01_04", "lh_wooden_chair_a_v1_bv01_05", "lh_wooden_chair_a_v1_bv01_06",
            "lh_wooden_clock_a_v1_bv01_01",
            "lh_wooden_crate_a_v1", "lh_wooden_crate_a_v1_bv01_00", "lh_wooden_crate_a_v1_bv01_01", "lh_wooden_crate_a_v1_bv01_05", "lh_wooden_crate_a_v1_bv01_11",
            "lh_wooden_crate_b_kit_v1_bv01", "lh_wooden_crate_b_v1", "lh_wooden_crate_b_v1_00", "lh_wooden_crate_b_v1_01", "lh_wooden_crate_b_v1_02",
            "lh_wooden_crate_b_v1_03", "lh_wooden_crate_b_v1_04", "lh_wooden_crate_b_v1_06", "lh_wooden_crate_b_v1_09", "lh_wooden_crate_b_v1_15", "lh_wooden_crate_b_v1_bv01_01",
            "lh_wooden_crate_c_v1", "lh_wooden_crate_c_v1_bv01_00",
            "lh_wooden_crate_d_v1_bv01_00", "lh_wooden_crate_d_v1_bv01_01", "lh_wooden_crate_d_v1_bv01_02", "lh_wooden_crate_d_v1_bv01_03", "lh_wooden_crate_d_v1_bv01_04",
            "lh_wooden_crate_d_v1_bv01_05", "lh_wooden_crate_d_v1_bv01_06", "lh_wooden_crate_d_v1_bv01_07", "lh_wooden_crate_d_v1_bv01_08", "lh_wooden_crate_d_v1_bv01_09",
            "lh_wooden_crate_d_v1_bv01_10", "lh_wooden_crate_d_v1_bv01_11", "lh_wooden_crate_d_v1_bv01_12", "lh_wooden_crate_d_v1_bv01_13", "lh_wooden_crate_d_v1_bv01_14",
            "lh_wooden_crate_d_v1_bv01_15", "lh_wooden_crate_d_v1_bv01_16", "lh_wooden_crate_d_v1_bv01_17", "lh_wooden_crate_d_v1_bv01_18", "lh_wooden_crate_d_v1_bv01_19",
            "lh_wooden_crate_d_v1_bv01_20", "lh_wooden_crate_d_v1_bv01_21", "lh_wooden_crate_d_v1_bv01_22", "lh_wooden_crate_d_v1_bv01_23", "lh_wooden_crate_d_v1_bv01_24",
            "lh_wooden_crate_d_v1_bv01_25", "lh_wooden_crate_d_v1_bv01_26", "lh_wooden_crate_d_v1_bv01_27", "lh_wooden_crate_d_v1_bv01_28", "lh_wooden_crate_d_v1_bv01_29",
            "lh_wooden_crate_d_v1_bv01_30", "lh_wooden_crate_d_v1_bv01_31", "lh_wooden_crate_d_v1_bv01_32", "lh_wooden_crate_d_v1_bv01_33", "lh_wooden_crate_d_v1_bv01_34",
            "lh_wooden_crate_d_v1_bv01_35", "lh_wooden_crate_d_v1_bv01_36", "lh_wooden_crate_d_v1_bv01_37", "lh_wooden_crate_d_v1_bv01_38", "lh_wooden_crate_d_v1_bv01_39",
            "lh_wooden_crate_d_v1_bv01_40", "lh_wooden_crate_d_v1_bv01_41", "lh_wooden_crate_d_v1_bv01_42", "lh_wooden_crate_d_v1_bv01_43", "lh_wooden_crate_d_v1_bv01_44",
            "lh_wooden_crate_d_v1_bv01_45", "lh_wooden_crate_d_v1_bv01_46", "lh_wooden_crate_d_v1_bv01_47", "lh_wooden_crate_d_v1_bv01_48", "lh_wooden_crate_d_v1_bv01_49",
            "lh_wooden_crate_d_v1_bv01_50", "lh_wooden_crate_d_v1_bv01_51", "lh_wooden_crate_d_v1_bv01_52", "lh_wooden_crate_d_v1_bv01_53", "lh_wooden_crate_d_v1_bv01_54",
            "lh_wooden_crate_d_v1_bv01_55", "lh_wooden_crate_d_v1_bv01_56", "lh_wooden_crate_d_v1_bv01_57", "lh_wooden_crate_d_v1_bv01_58",
            "lh_wooden_crates_a_v1", "lh_wooden_fence_breakable_a_v1", "lh_wooden_fence_breakable_d_v1",
            "lh_wooden_fence_d_v1_bv01_06", "lh_wooden_fence_d_v1_bv01_08", "lh_wooden_fence_d_v1_bv01_09",
            "lh_wooden_fire_poker_a_v1_bv01_00", "lh_wooden_fire_poker_a_v1_bv01_01", "lh_wooden_folding_ladder_a_v1_bv01_00", "lh_wooden_folding_ladder_b_v2",
            "lh_wooden_panel_a_v1_bv01_00", "lh_wooden_panel_a_v1_bv01_01", "lh_wooden_panel_b_v1_bv01_00",
            "lh_wooden_panels_a_destructible_v1_bv01_00", "lh_wooden_panels_a_destructible_v1_bv01_01", "lh_wooden_panels_a_destructible_v1_bv01_02",
            "lh_wooden_panels_a_destructible_v1_bv01_03", "lh_wooden_panels_a_destructible_v1_bv01_04", "lh_wooden_panels_a_destructible_v1_bv01_05",
            "lh_wooden_panels_a_destructible_v1_bv01_06", "lh_wooden_panels_a_destructible_v1_bv01_07", "lh_wooden_panels_a_destructible_v1_bv01_08",
            "lh_wooden_panels_a_destructible_v1_bv01_09", "lh_wooden_panels_a_destructible_v1_bv01_10",
            "lh_wooden_planks_pile_a_v1", "lh_wooden_shelf_a_v1_bv01_00", "lh_wooden_shelf_j_v1", "lh_wooden_shelve_a_v1", "lh_wooden_stool_a_v1",
            "lh_wooden_table_a_v1_bv01_00", "lh_wooden_table_d_v1_bv01_01", "lh_wooden_table_d_v1_bv01_02", "lh_wooden_table_g_v1",
            "vinvenzo_table",
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
