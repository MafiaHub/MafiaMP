#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include "shared/entities/vehicle_entity.h"

#include <scripting/builtins/color.h>
#include <scripting/builtins/entity.h>

#include <cstdint>
#include <memory>
#include <string>

namespace MafiaMP::Scripting {
    class Vehicle final: public Framework::Scripting::Builtins::Entity {
      public:
        Vehicle(uint64_t networkId);

        // Resolves the handle as a VehicleEntity, or nullptr if it is gone / not a vehicle.
        Shared::Entities::VehicleEntity *ResolveVehicle() const;

        // Arguments are the vehicle and player human NetworkIDs.
        static void EventVehiclePlayerEnter(uint64_t vehicleId, uint64_t playerId, int seatIndex);
        static void EventVehiclePlayerLeave(uint64_t vehicleId, uint64_t playerId);

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
        // Mutates the vehicle's replicated data, then lets the server have the last word over the
        // owner: ForceState pushes the change to the owning client (no-op for unowned vehicles, which
        // replicate the change to everyone through the DeltaSerializer instead).
        template <typename Mutator>
        void MutateData(Mutator &&mutate) {
            if (auto *v = ResolveVehicle()) {
                mutate(v->data);
                v->ForceState();
            }
        }

        static std::unique_ptr<v8pp::class_<Vehicle>> _class;
    };
} // namespace MafiaMP::Scripting
