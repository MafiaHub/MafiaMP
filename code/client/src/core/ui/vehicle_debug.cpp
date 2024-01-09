#include "vehicle_debug.h"
#include <external/imgui/wrapper.h>
#include <imgui.h>

#include "../../sdk/entities/c_car.h"
#include "../../sdk/entities/c_player_2.h"
#include "../../sdk/entities/c_vehicle.h"

#include "../../sdk/ue/sys/math/c_vector.h"

#include "game/helpers/controls.h"

namespace MafiaMP::Core::UI {
    VehicleDebug::VehicleDebug() {
    }

    void VehicleDebug::Update() {
        const auto pActivePlayer = Game::Helpers::Controls::GetLocalPlayer();
        SDK::C_Car *currentCar = pActivePlayer ? reinterpret_cast<SDK::C_Car *>(pActivePlayer->GetOwner()) : nullptr;
        if (currentCar) {
            auto currentVehicle = currentCar->GetVehicle();

            auto position = currentCar->GetPos();
            if (ImGui::SliderFloat3("Pos", (float *)&position, -50000.0, 50000.0)) {
                currentCar->SetPos(position);
            }

            auto rot = currentCar->GetRot();
            if (ImGui::SliderFloat4("Rot", (float *)&rot, -1.0, 1.0)) {
                currentCar->SetRot(rot);
            }

            auto velocity = currentVehicle->GetSpeed();
            if (ImGui::SliderFloat3("Speed", (float *)&velocity, -500.0, 500.0)) {
                currentVehicle->SetSpeed(velocity, false, false);
            }

            float isBrakeing = currentVehicle->GetBrake();
            if (ImGui::SliderFloat("Brake", &isBrakeing, 0.0f, 1.0f)) {
                currentVehicle->SetBrake(isBrakeing, false);
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
                currentCar->PosefujZimuVShopu(dirtLevel);
            }

            bool horn = currentVehicle->GetHorn();
            if (ImGui::Checkbox("Horn", &horn)) {
                currentVehicle->SetHorn(horn);
            }

            bool siren = currentVehicle->GetSiren();
            if (ImGui::Checkbox("Siren", &siren)) {
                currentVehicle->SetSiren(siren);
            }

            bool beaconsLight = currentVehicle->AreBeaconLightsOn();
            if (ImGui::Checkbox("Beacon Lights", &beaconsLight)) {
                currentVehicle->SetBeaconLightsOn(beaconsLight);
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

            auto wheelsTint = currentVehicle->GetWheelTintColor();
            if (ImGui::ColorEdit4("Wheels Tint", (float *)&wheelsTint)) {
                currentVehicle->SetWheelTintColor(wheelsTint);
            }

            SDK::ue::sys::math::C_Vector4 intColors[5];
            currentVehicle->GetInteriorColors(&intColors[0], &intColors[1], &intColors[2], &intColors[3], &intColors[4]);

            if (ImGui::ColorEdit4("Int color 1", (float *)&intColors[0]) || ImGui::ColorEdit4("Int color 2", (float *)&intColors[1]) || ImGui::ColorEdit4("Int color 3", (float *)&intColors[2]) || ImGui::ColorEdit4("Int color 4", (float *)&intColors[3])
                || ImGui::ColorEdit4("Int color 5", (float *)&intColors[4])) {
                currentVehicle->SetInteriorColors(&intColors[0], &intColors[1], &intColors[2], &intColors[3], &intColors[4]);
            }

            ImGui::Text("Fuel: %f\n", currentCar->GetActualFuel());
            currentCar->SetActualFuel(20.0);

            ImGui::Text("Damage: %f\n", currentCar->GetDamage());
            ImGui::Text("Motor damage: %f\n", currentCar->GetMotorDamage());

            float brake, handbrake;
            currentVehicle->GetDamageBrakes(brake, handbrake);
            ImGui::Text("Brake's damage: %f\n", brake);
            ImGui::Text("Handbrake's damage: %f\n", handbrake);

            ImGui::Text("Radio State: %s", currentVehicle->IsRadioOn() ? "On" : "Off");
            ImGui::Text("Radio Station: %u", currentVehicle->GetRadioStation());

            ImGui::Text("Car Ptr: %p", currentCar);
            ImGui::Text("Vehicle Ptr: %p", currentVehicle);

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

            if (ImGui::Button("Toggle Radio")) {
                currentVehicle->TurnRadioOn(!currentVehicle->IsRadioOn());
            }

            if (ImGui::Button("Change Radio")) {
                const uint32_t currentStation = currentVehicle->GetRadioStation();
                currentVehicle->ChangeRadioStation(currentStation == 1 ? 0 : 1);
            }
        }
    }
}; // namespace MafiaMP::Core::UI
