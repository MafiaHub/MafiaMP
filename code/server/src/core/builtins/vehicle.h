#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include "entity.h"

#include "core/modules/vehicle.h"

#include <scripting/builtins/color.h>

#include <fmt/format.h>

#include <memory>

namespace MafiaMP::Scripting {
    class Vehicle final: public Entity {
      public:
        Vehicle(flecs::entity_t ent): Entity(ent) {
            const auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();

            if (!vehData) {
                throw std::runtime_error(fmt::format("Entity handle '{}' is not a Vehicle!", ent));
            }
        }

        Vehicle(flecs::entity ent): Vehicle(ent.id()) {}

        static void EventVehiclePlayerEnter(flecs::entity vehicle, flecs::entity player, int seatIndex);
        static void EventVehiclePlayerLeave(flecs::entity vehicle, flecs::entity player);

        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global);
        static v8pp::class_<Vehicle> &GetClass(v8::Isolate *isolate);

        std::string ToString() const override;

        bool GetBeaconLightsOn();
        void SetBeaconLightsOn(bool on);

        Framework::Scripting::Builtins::Color GetColorPrimary();
        void SetColorPrimary(Framework::Scripting::Builtins::Color color);

        Framework::Scripting::Builtins::Color GetColorSecondary();
        void SetColorSecondary(Framework::Scripting::Builtins::Color color);

        float GetDirt();
        void SetDirt(float dirt);

        bool GetEngineOn();
        void SetEngineOn(bool on);

        float GetFuel();
        void SetFuel(float fuel);

        std::string GetLicensePlate();
        void SetLicensePlate(std::string plate);

        int GetLockState();
        void SetLockState(int state);

        bool GetRadioOn();
        void SetRadioOn(bool on);

        int GetRadioStationId();
        void SetRadioStationId(int id);

        Framework::Scripting::Builtins::Color GetRimColor();
        void SetRimColor(Framework::Scripting::Builtins::Color color);

        float GetRust();
        void SetRust(float rust);

        bool GetSirenOn();
        void SetSirenOn(bool on);

        Framework::Scripting::Builtins::Color GetTireColor();
        void SetTireColor(Framework::Scripting::Builtins::Color color);

        Framework::Scripting::Builtins::Color GetWindowTint();
        void SetWindowTint(Framework::Scripting::Builtins::Color tint);

      private:
        static std::unique_ptr<v8pp::class_<Vehicle>> _class;
    };
} // namespace MafiaMP::Scripting
