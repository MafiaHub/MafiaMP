#include "debug_vehicle.h"

#include <logging/logger.h>

#include "sdk/constants.h"
#include "sdk/entities/c_car.h"
#include "sdk/entities/c_car_tuning_manager.h"
#include "sdk/entities/c_player_2.h"
#include "sdk/ue/game/vehicle/c_vehicle.h"
#include "sdk/ue/sys/math/c_vector.h"
#include "sdk/ue/game/humanai/c_character_controller.h"
#include "sdk/wrappers/c_human_2_car_wrapper.h"

#include "game/helpers/controls.h"

namespace MafiaMP::Core::UI::Devs {
    void DebugVehicle::OnOpen() {}

    void DebugVehicle::OnClose() {}

    void DebugVehicle::OnUpdate() {
        const auto pActivePlayer = Game::Helpers::Controls::GetLocalPlayer();

        // Get current vehicle using the same pattern as the multiplayer hooks
        SDK::C_Car *currentCar = nullptr;
        if (pActivePlayer) {
            auto charCtrl = pActivePlayer->GetCharacterController();
            if (charCtrl) {
                auto carHandler = charCtrl->GetCarHandler();
                if (carHandler) {
                    auto carWrapper = carHandler->GetHuman2CarWrapper();
                    if (carWrapper) {
                        currentCar = carWrapper->m_pUsedCar;
                    }
                }
            }
        }

        auto windowContent = [&]() {
            if (!currentCar) {
                ImGui::Text("You're not in a vehicle!");
                return;
            }

            auto currentVehicle = currentCar->GetVehicle();
            if (!currentVehicle) {
                return;
            }

            if (ImGui::Button("Print Pointers")) {
                Framework::Logging::GetLogger("debug")->info("Car Ptr: 0x{}, Vehicle Ptr: 0x{}", fmt::ptr(currentCar), fmt::ptr(currentVehicle));
            }

            auto position = currentCar->GetPos();
            if (ImGui::DragFloat3("Pos", (float *)&position, 0.1f, -4500.0f, 4500.0f)) {
                currentCar->SetPos(position);
            }

            auto dir = currentCar->GetDir();
            if (ImGui::DragFloat3("Dir", (float *)&dir, 0.01f, -1.0f, 1.0f)) {
                currentCar->SetDir(dir);
            }

            auto rot = currentCar->GetRot();
            if (ImGui::DragFloat4("Rot", (float *)&rot, 0.01f, -1.0f, 1.0f)) {
                currentCar->SetRot(rot);
            }

            auto velocity = currentVehicle->GetSpeed();
            if (ImGui::SliderFloat3("Speed", (float *)&velocity, -500.0, 500.0)) {
                currentVehicle->SetSpeed(velocity, false, false);
            }

            float isBraking = currentVehicle->GetBrake();
            if (ImGui::SliderFloat("Brake", &isBraking, 0.0f, 1.0f)) {
                currentVehicle->SetBrake(isBraking, false);
            }

            float steering = currentVehicle->GetSteer();
            if (ImGui::SliderFloat("Steer", &steering, -5.0f, 5.0f)) {
                currentVehicle->SetSteer(steering);
            }

            float handBrake = currentVehicle->GetHandbrake();
            if (ImGui::SliderFloat("Handbrake", &handBrake, 0.0f, 1.0f)) {
                currentVehicle->SetHandbrake(handBrake, false);
            }

            int gear = currentCar->GetGear();
            if (ImGui::InputInt("Gear", &gear)) {
                currentVehicle->SetGear(gear);
            }

            float dirtLevel = currentVehicle->GetVehicleDirty();
            if (ImGui::SliderFloat("Dirt level", &dirtLevel, 0.0f, 1.0f)) {
                // We have to use the car to set the dirt otherwise the value is reset
                currentCar->SetVehicleDirty(dirtLevel);
            }

            float rustLevel = currentVehicle->GetVehicleRust();
            if (ImGui::SliderFloat("Rust level", &rustLevel, 0.0f, 1.0f)) {
                currentVehicle->SetVehicleRust(rustLevel);
            }

            const float fuelTankCapacity = currentCar->GetFuelTankCapacity();
            float fuelLevel              = currentCar->GetActualFuel();
            if (ImGui::SliderFloat("Fuel level", &fuelLevel, 0.0f, fuelTankCapacity)) {
                currentCar->SetActualFuel(fuelLevel);
            }

            const char *licensePlate = currentVehicle->GetSPZText();
            char wantedCurrentLicense[SDK::Constants::VEHICLE_LICENSE_PLATE_MAX_LENGTH];
            strcpy(wantedCurrentLicense, licensePlate);
            if (ImGui::InputText("License plate", wantedCurrentLicense, sizeof(wantedCurrentLicense))) {
                currentVehicle->SetSPZText(wantedCurrentLicense, true);
            }

            bool horn = currentVehicle->GetHorn();
            if (ImGui::Checkbox("Horn", &horn)) {
                currentVehicle->SetHorn(horn);
            }

            bool siren = currentVehicle->IsSiren();
            if (ImGui::Checkbox("Siren", &siren)) {
                currentVehicle->SetSiren(siren);
            }

            bool beaconsLight = currentVehicle->GetBeaconLightsOn();
            if (ImGui::Checkbox("Beacon Lights", &beaconsLight)) {
                currentVehicle->SetBeaconLightsOn(beaconsLight);
            }

            bool taxiLights = currentVehicle->GetTaxiLightsOn();
            if (ImGui::Checkbox("Taxi Lights", &taxiLights)) {
                currentVehicle->SetTaxiLightsOn(taxiLights);
            }

            bool isEngineOn = currentCar->IsEngineOn();
            if (ImGui::Checkbox("Engine", &isEngineOn)) {
                currentVehicle->SetEngineOn(isEngineOn, isEngineOn);
            }

            bool isLeftIndicatorOn = currentVehicle->IsIndicatorLightOn(SDK::ue::game::vehicle::E_VehicleIndicator::INDICATOR_LEFT);
            if (ImGui::Checkbox("Left Indicator", &isLeftIndicatorOn)) {
                currentVehicle->SetIndicatorLightsOn(isLeftIndicatorOn, SDK::ue::game::vehicle::E_VehicleIndicator::INDICATOR_LEFT);
            }

            bool isRightIndicatorOn = currentVehicle->IsIndicatorLightOn(SDK::ue::game::vehicle::E_VehicleIndicator::INDICATOR_RIGHT);
            if (ImGui::Checkbox("Right Indicator", &isRightIndicatorOn)) {
                currentVehicle->SetIndicatorLightsOn(isRightIndicatorOn, SDK::ue::game::vehicle::E_VehicleIndicator::INDICATOR_RIGHT);
            }

            SDK::ue::sys::math::C_Vector4 color1, color2;
            currentVehicle->GetVehicleColor(&color1, &color2);

            if (ImGui::ColorEdit4("Color 1", (float *)&color1)) {
                currentVehicle->SetVehicleColor(&color1, &color2, false);
            }

            if (ImGui::ColorEdit4("Color 2", (float *)&color2)) {
                currentVehicle->SetVehicleColor(&color1, &color2, false);
            }

            auto windowTint = currentVehicle->GetWindowTintColor();
            if (ImGui::ColorEdit4("Window Tint", (float *)&windowTint)) {
                currentVehicle->SetWindowTintColor(windowTint);
            }

            SDK::ue::sys::math::C_Vector4 rimColor, tireColor;
            currentVehicle->GetWheelColor(&rimColor, &tireColor);
            if (ImGui::ColorEdit4("Wheel Rim color", (float *)&rimColor)) {
                currentVehicle->SetWheelColor(&rimColor, &tireColor);
            }

            if (ImGui::ColorEdit4("Wheel Tire color", (float *)&tireColor)) {
                currentVehicle->SetWheelColor(&rimColor, &tireColor);
            }

            SDK::ue::sys::math::C_Vector4 intColors[5];
            currentVehicle->GetInteriorColors(&intColors[0], &intColors[1], &intColors[2], &intColors[3], &intColors[4]);

            if (ImGui::ColorEdit4("Int color 1", (float *)&intColors[0]) || ImGui::ColorEdit4("Int color 2", (float *)&intColors[1]) || ImGui::ColorEdit4("Int color 3", (float *)&intColors[2]) || ImGui::ColorEdit4("Int color 4", (float *)&intColors[3])
                || ImGui::ColorEdit4("Int color 5", (float *)&intColors[4])) {
                currentVehicle->SetInteriorColors(&intColors[0], &intColors[1], &intColors[2], &intColors[3], &intColors[4]);
            }

            ImGui::Text("Damage: %f\n", currentCar->GetDamage());
            ImGui::Text("Motor damage: %f\n", currentCar->GetMotorDamage());

            float brake, handbrake;
            currentVehicle->GetDamageBrakes(brake, handbrake);
            ImGui::Text("Brake's damage: %f\n", brake);
            ImGui::Text("Handbrake's damage: %f\n", handbrake);

            bool radioOn = currentVehicle->IsRadioOn();
            if (ImGui::Checkbox("Turn radio on", &radioOn)) {
                currentVehicle->TurnRadioOn(radioOn);
            }

            if (ImGui::Button("Change Radio")) {
                const uint32_t currentStation = currentVehicle->GetRadioStation();
                currentVehicle->ChangeRadioStation(currentStation == 1 ? 0 : 1);
            }

            // Tuning section
            if (ImGui::CollapsingHeader("Tuning")) {
                auto tuningMgr = currentCar->GetTuningManager();
                if (tuningMgr) {
                    ImGui::Text("Tuning Manager: 0x%p", tuningMgr);
                    ImGui::Text("Slot Count: %zu", tuningMgr->GetSlotCount());
                    ImGui::Text("Installed Items: %zu", tuningMgr->GetInstalledItemCount());

                    ImGui::Separator();

                    // Display installed tuning slots
                    if (ImGui::TreeNode("Installed Slots")) {
                        for (size_t i = 0; i < tuningMgr->GetSlotCount(); i++) {
                            auto slot = tuningMgr->GetSlot(i);
                            if (slot && slot->m_pCurrentItem) {
                                ImGui::Text("Slot %02zu (ID %d): Item ID %d",
                                    i,
                                    slot->m_uSlotId,
                                    slot->m_pCurrentItem->m_uItemId);
                            }
                            else if (slot) {
                                ImGui::TextDisabled("Slot %02zu (ID %d): Empty", i, slot->m_uSlotId);
                            }
                        }
                        ImGui::TreePop();
                    }

                    ImGui::Separator();

                    // Input for setting tuning items
                    static int tuningItemId = 0;
                    ImGui::InputInt("Tuning Item ID", &tuningItemId);

                    if (ImGui::Button("Apply Tuning Item")) {
                        tuningMgr->SetItemToSlot(static_cast<uint16_t>(tuningItemId));
                        currentCar->InstallTuningItems();
                        Framework::Logging::GetLogger("debug")->info("Applied tuning item: {}", tuningItemId);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Reinstall All Tuning")) {
                        currentCar->InstallTuningItems();
                        Framework::Logging::GetLogger("debug")->info("Reinstalled all tuning items");
                    }
                }
                else {
                    ImGui::TextDisabled("Tuning manager not available");
                }
            }

            ImGui::Separator();

            if (ImGui::Button("Restore")) {
                currentCar->RestoreCar();
            }

            if (ImGui::Button("Explode")) {
                currentCar->ExplodeCar();
            }

            if (ImGui::Button("LockEntryPoints")) {
                currentCar->LockEntryPoints();
            }

            if (ImGui::Button("UnlockEntryPoints")) {
                currentCar->UnlockEntryPoints();
            }
        };

        CreateUIWindow("Vehicle debug", windowContent, &_open, ImGuiWindowFlags_AlwaysAutoResize);
    }
}; // namespace MafiaMP::Core::UI::Devs
