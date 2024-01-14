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

        bool GetBeaconLightsEnabled() {
            auto syncData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            return syncData->beaconLightsState;
        }

        void SetBeaconLightsEnabled(bool state) {
            auto carData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            carData->beaconLightsState = state;
            MafiaMP::Shared::RPC::VehicleSetProps msg {};
            msg.beaconLightsState = state;
            FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
        }

        std::string GetLicensePlate() {
            auto carData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            return carData->licensePlate;
        }

        void SetLicensePlate(std::string plate) {
            auto carData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            ::memcpy(carData->licensePlate, plate.c_str(), std::min(sizeof(carData->licensePlate), plate.length()));
            MafiaMP::Shared::RPC::VehicleSetProps msg {};
            msg.licensePlate = plate.c_str();
            FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
        }

        Shared::Modules::VehicleSync::LockState GetLockState() {
            auto carData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            return carData->locked;
        }

        void Lock(Shared::Modules::VehicleSync::LockState state) {
            auto carData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            carData->locked = state;
            MafiaMP::Shared::RPC::VehicleSetProps msg {};
            msg.locked = state;
            FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
        }

        bool GetRadioEnabled() {
            auto syncData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            return syncData->radioState;
        }

        void SetRadioEnabled(bool state) {
            auto carData     = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            carData->radioState = state;
            MafiaMP::Shared::RPC::VehicleSetProps msg {};
            msg.radioState = state;
            FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
        }

        int GetRadioStation() {
            auto syncData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            return syncData->radioId;
        }

        void SetRadioStation(int id) {
            auto carData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            carData->radioId = id;
            MafiaMP::Shared::RPC::VehicleSetProps msg{};
            msg.radioId = id;
            FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
        }

        bool GetSirenEnabled() {
            auto syncData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            return syncData->sirenState;
        }

       void SetSirenEnabled(bool state) {
            auto carData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            carData->sirenState = state;
            MafiaMP::Shared::RPC::VehicleSetProps msg {};
            msg.sirenState = state;
            FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
        }

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            if (!rootModule) {
                return;
            }

            v8pp::class_<Vehicle> cls(isolate);
            cls.inherit<Framework::Integrations::Scripting::Entity>();
            cls.function("getBeaconLightsEnabled", &Vehicle::GetBeaconLightsEnabled);
            cls.function("setBeaconLightsEnabled", &Vehicle::SetBeaconLightsEnabled);
            cls.function("getLicensePlate", &Vehicle::GetLicensePlate);
            cls.function("setLicensePlate", &Vehicle::SetLicensePlate);
            cls.function("getLockState", &Vehicle::GetLockState);
            cls.function("setLockState", &Vehicle::Lock);
            cls.function("getRadioEnabled", &Vehicle::GetRadioEnabled);
            cls.function("setRadioEnabled", &Vehicle::SetRadioEnabled);
            cls.function("getRadioStation", &Vehicle::GetRadioStation);
            cls.function("setRadioStation", &Vehicle::SetRadioStation);
            cls.function("getSirenEnabled", &Vehicle::GetSirenEnabled);
            cls.function("setSirenEnabled", &Vehicle::SetSirenEnabled);
            rootModule->class_("Vehicle", cls);
        }
    };
} // namespace MafiaMP::Scripting
