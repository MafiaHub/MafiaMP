#pragma once

#include "integrations/server/scripting/builtins/node/entity.h"
#include "scripting/engines/node/engine.h"
#include "scripting/engines/node/sdk.h"
#include "shared/modules/vehicle_sync.hpp"
#include "core/modules/vehicle.h"
#include "shared/game_rpc/set_vehicledata.h"
#include "shared/game_rpc/vehicle/vehicle_setprops.h"

namespace MafiaMP::Scripting {
    class Vehicle final: public Framework::Integrations::Scripting::Entity {
      public:
        Vehicle(flecs::entity_t ent): Entity(ent) {}

        std::string ToString() const override {
            std::ostringstream ss;
            ss << "Vehicle{ id: " << _ent.id() << " }";
            return ss.str();
        }

        void Lock(Shared::Modules::VehicleSync::LockState state) {
            auto carData = _ent.get_mut<Core::Modules::Vehicle::CarData>();
            carData->locked = state;
        }

        Shared::Modules::VehicleSync::LockState GetLockState() {
            auto carData = _ent.get_mut<Core::Modules::Vehicle::CarData>();
            return carData->locked;
        }

        void SetLicensePlate(std::string plate) {
            auto carData = _ent.get_mut<Core::Modules::Vehicle::CarData>();
            carData->licensePlate = plate;
        }

        std::string GetLicensePlate() {
            auto carData = _ent.get_mut<Core::Modules::Vehicle::CarData>();
            return carData->licensePlate;
        }

        void SetSiren(bool state) {
            auto syncData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            syncData->siren = state;
        }

        bool GetSiren() {
            auto syncData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            return syncData->siren;
        }

        bool IsRadioOn() {
            auto syncData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            return syncData->radioId != -1;
        }

        void ChangeRadioStation(int id) {
            MafiaMP::Shared::RPC::VehicleSetProps msg{};
            msg.radioId = id;
            FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
        }

        void TurnOffRadio() {
            MafiaMP::Shared::RPC::VehicleSetProps msg {};
            msg.radioId = -1;
            FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
        }

        void SetBeaconLights(bool state) {
            auto syncData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            syncData->beaconLights = state;
        }

        bool GetBeaconLights() {
            auto syncData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            return syncData->beaconLights;
        }

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            if (!rootModule) {
                return;
            }

            v8pp::class_<Vehicle> cls(isolate);
            cls.inherit<Framework::Integrations::Scripting::Entity>();
            cls.function("setLockState", &Vehicle::Lock);
            cls.function("getLockState", &Vehicle::GetLockState);
            cls.function("setLicensePlate", &Vehicle::SetLicensePlate);
            cls.function("getLicensePlate", &Vehicle::GetLicensePlate);
            cls.function("setSiren", &Vehicle::SetSiren);
            cls.function("getSiren", &Vehicle::GetSiren);
            cls.function("isRadioOn", &Vehicle::IsRadioOn);
            cls.function("changeRadioStation", &Vehicle::ChangeRadioStation);
            cls.function("turnOffRadio", &Vehicle::TurnOffRadio);
            cls.function("setBeaconLights", &Vehicle::SetBeaconLights);
            cls.function("getBeaconLights", &Vehicle::GetBeaconLights);
            rootModule->class_("Vehicle", cls);
        }
    };
} // namespace MafiaMP::Scripting
