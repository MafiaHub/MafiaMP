#pragma once

#include "core/modules/vehicle.h"

#include "integrations/server/scripting/builtins/node/entity.h"

#include "scripting/engines/node/builtins/color_rgb.h"
#include "scripting/engines/node/builtins/color_rgba.h"
//#include "scripting/engines/node/engine.h"
//#include "scripting/engines/node/sdk.h"
//#include "shared/modules/vehicle_sync.hpp"
//#include "core/modules/vehicle.h"
//#include "shared/game_rpc/vehicle/vehicle_setprops.h"
//#include "shared/modules/vehicle_sync.hpp"

namespace MafiaMP::Scripting {
    class Vehicle final: public Framework::Integrations::Scripting::Entity {
      public:
        Vehicle(flecs::entity_t ent): Entity(ent) {}

        static void EventVehiclePlayerEnter(flecs::entity vehicle, flecs::entity player, int seatIndex);
        static void EventVehiclePlayerLeave(flecs::entity vehicle, flecs::entity player);

        std::string ToString() const override;

        bool GetBeaconLightsOn();
        void SetBeaconLightsOn(bool on);

        v8::Local<v8::Object> GetColorPrimary();
        void SetColorPrimary(Framework::Scripting::Engines::Node::Builtins::ColorRGB rgb);

        v8::Local<v8::Object> GetColorSecondary();
        void SetColorSecondary(Framework::Scripting::Engines::Node::Builtins::ColorRGB rgb);

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

        v8::Local<v8::Object> GetRimColor();
        void SetRimColor(Framework::Scripting::Engines::Node::Builtins::ColorRGB rgb);

        float GetRust();
        void SetRust(float rust);

        bool GetSirenOn();
        void SetSirenOn(bool on);

        v8::Local<v8::Object> GetTireColor();
        void SetTireColor(Framework::Scripting::Engines::Node::Builtins::ColorRGB rgb);

        v8::Local<v8::Object> GetWindowTint();
        void SetWindowTint(Framework::Scripting::Engines::Node::Builtins::ColorRGBA tint);

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            if (!rootModule) {
                return;
            }

            v8pp::class_<Vehicle> cls(isolate);
            cls.inherit<Framework::Integrations::Scripting::Entity>();

            cls.function("getBeaconLightsOn", &Vehicle::GetBeaconLightsOn);
            cls.function("getColorPrimary", &Vehicle::GetColorPrimary);
            cls.function("getColorSecondary", &Vehicle::GetColorSecondary);
            cls.function("getDirt", &Vehicle::GetDirt);
            cls.function("getEngineOn", &Vehicle::GetEngineOn);
            cls.function("getFuel", &Vehicle::GetFuel);
            cls.function("getLicensePlate", &Vehicle::GetLicensePlate);
            cls.function("getLockState", &Vehicle::GetLockState);
            cls.function("getRadioOn", &Vehicle::GetRadioOn);
            cls.function("getRadioStationId", &Vehicle::GetRadioStationId);
            cls.function("getRimColor", &Vehicle::GetRimColor);
            cls.function("getRust", &Vehicle::GetRust);
            cls.function("getSirenOn", &Vehicle::GetSirenOn);
            cls.function("getTireColor", &Vehicle::GetTireColor);
            cls.function("getWindowTint", &Vehicle::GetWindowTint);
            cls.function("setBeaconLightsOn", &Vehicle::SetBeaconLightsOn);
            cls.function("setColorPrimary", &Vehicle::SetColorPrimary);
            cls.function("setColorSecondary", &Vehicle::SetColorSecondary);
            cls.function("setDirt", &Vehicle::SetDirt);
            cls.function("setEngineOn", &Vehicle::SetEngineOn);
            cls.function("setFuel", &Vehicle::SetFuel);
            cls.function("setLicensePlate", &Vehicle::SetLicensePlate);
            cls.function("setLockState", &Vehicle::SetLockState);
            cls.function("setRadioOn", &Vehicle::SetRadioOn);
            cls.function("setRadioStationId", &Vehicle::SetRadioStationId);
            cls.function("setRimColor", &Vehicle::SetRimColor);
            cls.function("setRust", &Vehicle::SetRust);
            cls.function("setSirenOn", &Vehicle::SetSirenOn);
            cls.function("setTireColor", &Vehicle::SetTireColor);
            cls.function("setWindowTint", &Vehicle::SetWindowTint);

            rootModule->class_("Vehicle", cls);
        }
    };
} // namespace MafiaMP::Scripting
