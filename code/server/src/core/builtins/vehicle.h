#pragma once

#include <sol/sol.hpp>

#include "core/modules/vehicle.h"

#include "integrations/server/scripting/builtins/entity.h"

#include "scripting/server_engine.h"
#include "scripting/builtins/color_rgb.h"
#include "scripting/builtins/color_rgba.h"

namespace MafiaMP::Scripting {
    class Vehicle final: public Framework::Integrations::Scripting::Entity {
      public:
        Vehicle(flecs::entity_t ent) : Entity(ent) {
            const auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();

            if (!vehData) {
                throw std::runtime_error(fmt::format("Entity handle '{}' is not a Vehicle!", ent));
            }
        }
        
        Vehicle(flecs::entity ent): Vehicle(ent.id()) {}

        static void EventVehiclePlayerEnter(flecs::entity vehicle, flecs::entity player, int seatIndex);
        static void EventVehiclePlayerLeave(flecs::entity vehicle, flecs::entity player);

        static void Register(sol::state &luaEngine);

        std::string ToString() const override;

        bool GetBeaconLightsOn();
        void SetBeaconLightsOn(bool on);

        Framework::Scripting::Builtins::ColorRGB GetColorPrimary();
        void SetColorPrimary(Framework::Scripting::Builtins::ColorRGB rgb);

        Framework::Scripting::Builtins::ColorRGB GetColorSecondary();
        void SetColorSecondary(Framework::Scripting::Builtins::ColorRGB rgb);

        float GetDirt();
        void SetDirt(float dirt);

        bool GetEngineOn();
        void SetEngineOn(bool on);

        float GetFuel();
        void SetFuel(float fuel);

        std::string GetLicensePlate();
        void SetLicensePlate(std::string plate);

        Shared::Modules::VehicleSync::LockState GetLockState();
        void SetLockState(Shared::Modules::VehicleSync::LockState state);

        bool GetRadioOn();
        void SetRadioOn(bool on);

        int GetRadioStationId();
        void SetRadioStationId(int id);

        Framework::Scripting::Builtins::ColorRGB GetRimColor();
        void SetRimColor(Framework::Scripting::Builtins::ColorRGB rgb);

        float GetRust();
        void SetRust(float rust);

        bool GetSirenOn();
        void SetSirenOn(bool on);

        Framework::Scripting::Builtins::ColorRGB GetTireColor();
        void SetTireColor(Framework::Scripting::Builtins::ColorRGB rgb);

        Framework::Scripting::Builtins::ColorRGBA GetWindowTint();
        void SetWindowTint(Framework::Scripting::Builtins::ColorRGBA tint);


    };
} // namespace MafiaMP::Scripting
