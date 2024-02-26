#pragma once

#include "core/modules/vehicle.h"

#include "integrations/server/scripting/builtins/node/entity.h"

#include "scripting/engines/node/builtins/color_rgb.h"
#include "scripting/engines/node/builtins/color_rgba.h"

namespace MafiaMP::Scripting {
    class Vehicle final: public Framework::Integrations::Scripting::Entity {
      public:
        Vehicle(flecs::entity_t ent): Entity(ent) {}

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

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule);

        static v8::Local<v8::Object> WrapVehicle(v8::Isolate *isolate, flecs::entity e);
    };
} // namespace MafiaMP::Scripting
