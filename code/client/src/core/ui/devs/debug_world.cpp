#include "debug_world.h"

#include <logging/logger.h>

#include "core/application.h"

#include "sdk/c_game_gfx_env_eff_module.h"
#include "sdk/entities/c_car.h"
#include "sdk/entities/c_player_2.h"
#include "sdk/entities/c_vehicle.h"
#include "sdk/mafia/database/c_ui_database.h"
#include "sdk/mafia/framework/c_mafia_dbs.h"
#include "sdk/mafia/ui/c_game_gui_2_module.h"
#include "sdk/mafia/ui/hud/race_xbin.h"
#include "sdk/mafia/ui/hud/c_hud_controller.h"
#include "sdk/ue/game/traffic/c_streaming_traffic_module.h"
#include "sdk/ue/gfx/environmenteffects/c_gfx_environment_effects.h"

#include "game/helpers/controls.h"
#include "game/helpers/human.h"

namespace MafiaMP::Core::UI::Devs {
    void DebugWorld::OnOpen() {}

    void DebugWorld::OnClose() {}

    void DebugWorld::OnUpdate() {
        const auto pActivePlayer = Game::Helpers::Controls::GetLocalPlayer();
        if (!pActivePlayer) {
            return;
        }

        const auto gfxEnvironmentEffects = SDK::ue::gfx::environmenteffects::C_GfxEnvironmentEffects::GetInstance();
        if (!gfxEnvironmentEffects) {
            return;
        }
        const auto weatherManager = gfxEnvironmentEffects->GetWeatherManager();
        if (!weatherManager) {
            return;
        }

        const auto streamingTrafficModule = SDK::ue::game::traffic::C_StreamingTrafficModule::GetInstance();
        if (!streamingTrafficModule) {
            return;
        }

        auto windowContent = [&]() {
            if (ImGui::CollapsingHeader("Vehicle spawner")) {
                if (ImGui::Button("Despawn all")) {
                    for (const auto &vehicle : _spawnedVehicles) {
                        gApplication->GetEntityFactory()->ReturnEntity(vehicle);
                    }
                    _spawnedVehicles.clear();
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                static const char *_selectedVehicleModelName = "berkley_810";

                static bool _showSupportedOnly = true;
                ImGui::Checkbox("Show supported by MafiaMP only", &_showSupportedOnly);

                static char _modelNameFilter[100] = "";
                ImGui::InputText("ModelName filter", _modelNameFilter, 100);

                {
                    const auto mafiaDB = SDK::mafia::framework::GetMafiaDBs();
                    if (!mafiaDB) {
                        return;
                    }

                    const auto vehiclesDB = mafiaDB->GetVehiclesDatabase();
                    if (!vehiclesDB.IsValid()) {
                        return;
                    }

                    const auto vehiclesDBCount = vehiclesDB->GetVehiclesCount();

                    if (ImGui::BeginListBox("Vehicle list")) {
                        for (int n = 0; n < vehiclesDBCount; n++) {
                            using namespace SDK::mafia::framework;
                            const C_VehiclesDatabase::TItemAccessorConst &vehicleAccessor = vehiclesDB->GetVehicleByIndex(n);

                            if (const auto *vehicle = vehicleAccessor.Get()) {
                                const auto vehicleID = vehicle->GetID();

                                // Unused index, nothing to spawn
                                if (vehicle->GetID() == 0) {
                                    continue;
                                }

                                const char *modelName = vehicle->GetModelName();
                                if (!modelName) {
                                    Framework::Logging::GetLogger("Debug")->info("Skipping ID {}, encountered m_ModelName which is nullptr", vehicleID);
                                    continue;
                                }

                                if (_showSupportedOnly && vehicle->HasVehicleFlags(SDK::mafia::traffic::E_TrafficVehicleFlags::E_TVF_CAR) == false) {
                                    continue;
                                }

                                if (strlen(_modelNameFilter) > 0 && strstr(modelName, _modelNameFilter) == nullptr) {
                                    continue;
                                }

                                const bool isSelected = (strcmp(_selectedVehicleModelName, modelName) == 0);

                                auto itemName = fmt::format("{} (id: {})", modelName, vehicleID);
                                if (ImGui::Selectable(itemName.c_str(), isSelected)) {
                                    _selectedVehicleModelName = modelName;
                                }

                                if (isSelected) {
                                    ImGui::SetItemDefaultFocus();
                                }
                            }
                        }

                        ImGui::EndListBox();
                    }
                }

                static bool _putPlayerIn = true;
                ImGui::Checkbox("Put player in", &_putPlayerIn);

                static bool _replacePreviousVehicleSpawned = true;
                ImGui::Checkbox("Replace previous vehicle spawned", &_replacePreviousVehicleSpawned);

                if (ImGui::Button("Spawn")) {
                    SpawnCar(_selectedVehicleModelName, _putPlayerIn, _replacePreviousVehicleSpawned);
                }
            }

            if (ImGui::CollapsingHeader("Weather Set")) {
                static float _weatherSetTransitionSpeed = 1.0f;
                ImGui::InputFloat("Transition speed", &_weatherSetTransitionSpeed);

                static const char *_weatherSetNames[] = {"__test_all_cloud", "__test_cloud", "__test_high_cloud", "_default_editor", "_default_game", "_default_game_cloudy", "_default_game_foggy", "_default_game_morning_sunny", "_default_game_overcast", "_default_game_rainy",
                    "_test_jakub", "_time_of_day", "cine_0495_intermission_one_morello", "cine_0997", "cine_1195_omerta_funeral_night", "cine_1195_omerta_funeral_night_emiss_off", "cine_1700_afternoon2", "cine_1700_night_rain", "cine_1700_noon", "cine_1700_sunset",
                    "day_cycle_01", "lh_gui_menu_default", "main_menu", "mm_000_prolgue_cp_240_norman_stairs", "mm_000_prologue_cp_000", "mm_000_prologue_cp_00_lighthouse_cliff", "mm_000_prologue_cp_00_lihgthouse", "mm_000_prologue_cp_050_church_open",
                    "mm_000_prologue_cp_060_church_edge", "mm_000_prologue_cp_070_church_city", "mm_000_prologue_cp_080_city_look_up", "mm_000_prologue_cp_090_city_look_down", "mm_000_prologue_cp_100_italy_metro", "mm_000_prologue_cp_110_italy_market",
                    "mm_000_prologue_cp_120_italy_street", "mm_000_prologue_cp_130_city_to_work", "mm_000_prologue_cp_150_work_boat", "mm_000_prologue_cp_170_work_truck", "mm_000_prologue_cp_180_crowd", "mm_000_prologue_cp_190_boat_reveal", "mm_000_prologue_cp_200_boat_boat",
                    "mm_000_prologue_cp_210_boat_bridge", "mm_000_prologue_cp_220_norman_bridge", "mm_000_prologue_cp_260_crowd", "mm_000_prologue_cp_300_norman_feet", "mm_00_prologue_cp_00_lighthouse_car", "mm_00_prologue_cp_00_lighthouse_house", "mm_010_chase_cp_020_escape",
                    "mm_010_chase_cp_020_escape_gi_off", "mm_020_taxi_cp_010_arrival", "mm_020_taxi_cp_010_arrival_gi_off", "mm_020_taxi_cp_070_cine_0200_taxi", "mm_030_molotov_cp_010_cine", "mm_030_molotov_cp_010_cinematic_intro", "mm_040_motel_cp_005_meet_salieri",
                    "mm_050_race_cp_010", "mm_050_race_cp_110", "mm_050_race_cp_120", "mm_050_race_cp_140", "mm_060_sarah_cp_010_cine_0600_sarah_intro", "mm_070_hoodlums_cp_010", "mm_070_hoodlums_cp_086_cover_paulie", "mm_080_brothel_cp_010_cs_start",
                    "mm_080_brothel_cp_110_cine_0850_brothel_funera", "mm_100_farm_cp_000", "mm_100_farm_cp_050", "mm_100_farm_cp_080", "mm_100_farm_cp_140", "mm_110_omerta_cp_010_cs_cs_park", "mm_110_omerta_cp_050_cs_safehouse", "mm_110_omerta_cp_120_cine_funeral",
                    "mm_110_omerta_cp_120_cs_funeral", "mm_120_mansion_cp_010_cs_salvatore", "mm_130_parking_cp_010_cine_parking", "mm_140_salieri_cp_010_cine_salieri", "mm_140_salieri_cp_100_salieri_outro", "mm_150_boat_cp_010", "mm_150_boat_cp_080_explore_the_ship",
                    "mm_150_boat_cp_120", "mm_160_harbor_cp_000_cinematic_night", "mm_160_harbor_cp_010_cinematic_start", "mm_160_harbor_cp_080_harbour_entrance_cutscene", "mm_170_plane_cp_010_cine_1700_plane", "mm_170_plane_cp_060_cine_1750_plane_airport",
                    "mm_170_plane_cp_080_cine_1760_plane_shoot", "mm_180_sniper_cp_010", "mm_180_sniper_cp_010_2", "mm_190_cigars_cp_010_cs_intro", "mm_190_cigars_cp_030_drive_to_harbor", "mm_200_bank_cp_010_bank_arrival", "mm_210_gallery_cp_010", "mm_210_gallery_cp_050",
                    "mm_cine_0005_prologue", "mm_cine_0995_intermission_two", "mm_cine_1395_intermission_three", "mm_cine_1795_intermission_four", "mm_prologue_cp_290_cross_road", "outro_cine_2210_salieri", "outro_cine_2210_tommy_death", "outro_cine_2210_tommy_prison",
                    "outro_cine_2210_vincenzo_ralph", "outro_cine_2210_wedding", "temp_teaser_trailer", "test_for_screenshots_alpha", "test_no_color_grading_day", "test_no_color_grading_night", "test_no_color_grading_overcast", "trailer_comp_02", "trailer_comp_02b",
                    "trailer_comp_02c", "trailer_comp_03"};

                const char *currentWeatherSet = SDK::C_GameGfxEnvEffModule::GetCurrentWeatherSetName();
                if (ImGui::BeginCombo("Name", currentWeatherSet)) {
                    for (int n = 0; n < IM_ARRAYSIZE(_weatherSetNames); n++) {
                        const bool isSelected = (strcmp(currentWeatherSet, _weatherSetNames[n]) == 0);

                        if (ImGui::Selectable(_weatherSetNames[n], isSelected)) {
                            std::string selectedItem(_weatherSetNames[n]);
                            weatherManager->SetWeatherSet(selectedItem.c_str(), _weatherSetTransitionSpeed);
                        }

                        if (isSelected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
            }

            if (ImGui::CollapsingHeader("Time")) {
                float defaultTimeFlowSpeedMult = weatherManager->GetDefaultTimeFlowSpeedMult();
                if (ImGui::SliderFloat("DefaultTimeFlowSpeedMult", &defaultTimeFlowSpeedMult, 0.0f, 100.0f)) {
                    weatherManager->SetDefaultTimeFlowSpeedMult(defaultTimeFlowSpeedMult);
                }

                float userTimeFlowSpeedMult = weatherManager->GetUserTimeFlowSpeedMult();
                if (ImGui::SliderFloat("UserTimeFlowSpeedMult", &userTimeFlowSpeedMult, 0.0f, 100.0f)) {
                    weatherManager->SetUserTimeFlowSpeedMult(userTimeFlowSpeedMult);
                }

                bool isTimeFlowEnabled = weatherManager->IsTimeFlowEnabled();
                if (ImGui::Checkbox("Enable TimeFlow", &isTimeFlowEnabled)) {
                    weatherManager->EnableTimeFlow(isTimeFlowEnabled);
                }

                float dayTimeHours = weatherManager->GetDayTimeHours();
                if (ImGui::SliderFloat("DayTimeHours", &dayTimeHours, 0.0f, 24.0f)) {
                    weatherManager->SetDayTimeHours(dayTimeHours);
                }

                float dayTimeRel = weatherManager->GetDayTimeRel();
                ImGui::Text("DayTimeRel: %f", dayTimeRel);

                float dayTimeSec = *reinterpret_cast<float *>(weatherManager + 3212);
                if (ImGui::InputFloat("DayTimeSec", &dayTimeSec)) {
                    weatherManager->SetDayTimeSec(dayTimeSec);
                }
            }

            if (ImGui::CollapsingHeader("Season")) {
                if (ImGui::Button("Close Season")) {
                    streamingTrafficModule->CloseSeason(true);
                }

                bool isSeasonOpened = streamingTrafficModule->GetSeasonOpened();
                ImGui::Text("Is Season Opened: %s\n", isSeasonOpened ? "true" : "false");

                ImGui::Text("PreviousSeasonID: %i\n", streamingTrafficModule->GetPreviousSeasonID());

                int currentSeasonID = streamingTrafficModule->GetCurrentSeasonID();
                ImGui::Text("CurrentSeasonID: %i\n", currentSeasonID);

                static const std::map<const char *, int> _seasonMap = {{"No season", -1}, {"1930", 1}, {"1932", 2}, {"1933", 3}, {"1935", 4}, {"1938", 5}, {"Freeride", 100}};

                auto foundSeasonID = std::find_if(_seasonMap.begin(), _seasonMap.end(), [currentSeasonID](const auto &pair) {
                    return pair.second == currentSeasonID;
                });

                const char *selectedSeasonName = (foundSeasonID != _seasonMap.end()) ? foundSeasonID->first : nullptr;

                if (ImGui::BeginCombo("##seasonId", selectedSeasonName)) {
                    for (auto it = _seasonMap.begin(); it != _seasonMap.end(); ++it) {
                        const bool isSelected = (currentSeasonID == it->second);

                        if (ImGui::Selectable(it->first, isSelected)) {
                            streamingTrafficModule->CloseSeason(true);
                            streamingTrafficModule->OpenSeason(it->second, true);
                        }

                        if (isSelected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }

                    ImGui::EndCombo();
                }
            }

            if (ImGui::CollapsingHeader("Traffic")) {
                int m_iMaxHumanElements = streamingTrafficModule->GetMaxHumanElements();
                if (ImGui::InputInt("MaxHumanElements", &m_iMaxHumanElements)) {
                    streamingTrafficModule->SetMaxHumanElements(m_iMaxHumanElements);
                }
            }
            
            if (ImGui::CollapsingHeader("Racing")) {
                SDK::mafia::ui::C_GameGUI2Module *GameGuiModule = SDK::mafia::ui::GetGameGui2Module();
                SDK::ue::C_WeakPtr<SDK::ue::sys::sodb::C_DatabaseInterface> result = GameGuiModule->GetDatabase();
                if (SDK::mafia::database::C_UIDatabase *database = reinterpret_cast<SDK::mafia::database::C_UIDatabase *>(result.Get())) {
                    SDK::mafia::database::C_UIDatabase::C_HUDTable *hudTable = database->GetHUDTable();

                    ImGui::PushItemWidth(75.0f);

                    // VISIBLE
                    ImGui::Text("Racing HUDElement Visibility");
                    ImGui::Checkbox("##racing_visible_hudtable", &hudTable->m_bRacingVisible);

                    ImGui::Spacing();

                    // LAPS
                    ImGui::Text("Current Laps / Num Laps");
                    ImGui::InputScalar("##total_laps_hudtable", ImGuiDataType_U16, &hudTable->m_uCurLap);
                    ImGui::SameLine();
                    ImGui::InputScalar("##curent_lap_hudtable", ImGuiDataType_U16, &hudTable->m_uTotalLaps);

                    ImGui::Spacing();

                    // POSITIONS
                    ImGui::Text("Current Position / Max Position");
                    ImGui::InputScalar("##total_position_hudtable", ImGuiDataType_U16, &hudTable->m_uCurPosition);
                    ImGui::SameLine();
                    ImGui::InputScalar("##curent_position_hudtable", ImGuiDataType_U16, &hudTable->m_uTotalPositions);

                    ImGui::Spacing();

                    // CHECKPOINTS
                    ImGui::Text("Current Checkpoint / Max Checkpoints");
                    ImGui::InputScalar("##total_checkpoint_hudtable", ImGuiDataType_U16, &hudTable->m_uCurCheckpoint);
                    ImGui::SameLine();
                    ImGui::InputScalar("##curent_checkpoint_hudtable", ImGuiDataType_U16, &hudTable->m_uTotalCheckpoints);

                    ImGui::Spacing();

                    // UNKNOWN
                    ImGui::Text("Current UNKNOWN / Max UNKNOWN");
                    ImGui::InputScalar("##total_UNKNOWN_hudtable", ImGuiDataType_U16, &hudTable->m_uUnknown1);
                    ImGui::SameLine();
                    ImGui::InputScalar("##curent_UNKNOWN_hudtable", ImGuiDataType_U16, &hudTable->m_uUnknown2);

                    ImGui::Spacing();

                    // COUNTDOWN
                    ImGui::Text("Countdown");
                    ImGui::SameLine();
                    ImGui::TextDisabled("(3, 2, 1 for lights, 0 for GO). Sound is played automatically");
                    ImGui::InputScalar("##countdown_hudtable", ImGuiDataType_U8, &hudTable->m_uCountdown);

                    ImGui::PopItemWidth();
                }
            }
        };

        CreateUIWindow("World debug", windowContent, &_open);
    }

    void DebugWorld::SpawnCar(std::string modelName, bool putPlayerIn, bool replacePrevious) {
        auto info = gApplication->GetEntityFactory()->RequestVehicle(modelName);

        if (replacePrevious && !_spawnedVehicles.empty()) {
            auto previousVehicle = _spawnedVehicles.back();
            gApplication->GetEntityFactory()->ReturnEntity(previousVehicle);
            _spawnedVehicles.pop_back();
        }

        _spawnedVehicles.push_back(info);

        const auto OnCarRequestFinish = [putPlayerIn](Game::Streaming::EntityTrackingInfo *info, bool success) {
            if (success) {
                auto car = reinterpret_cast<SDK::C_Car *>(info->GetEntity());
                if (!car) {
                    return;
                }
                car->GameInit();
                car->Activate();
                car->Unlock();

                auto localPlayer = Game::Helpers::Controls::GetLocalPlayer();

                SDK::ue::sys::math::C_Vector newPos    = localPlayer->GetPos();
                SDK::ue::sys::math::C_Quat newRot      = localPlayer->GetRot();
                SDK::ue::sys::math::C_Matrix transform = {};
                transform.Identity();
                transform.SetRot(newRot);
                transform.SetPos(newPos);
                car->GetVehicle()->SetVehicleMatrix(transform, SDK::ue::sys::core::E_TransformChangeType::DEFAULT);
                car->GetVehicle()->SetSPZText("DEBUG", true);

                if (putPlayerIn) {
                    auto characterController = reinterpret_cast<MafiaMP::Game::Overrides::CharacterController *>(localPlayer->GetCharacterController());
                    Game::Helpers::Human::PutIntoCar(characterController, car, 0, true);
                }
            }
        };

        const auto OnCarReturned = [](Game::Streaming::EntityTrackingInfo *info, bool wasCreated) {
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
}; // namespace MafiaMP::Core::UI::Devs
