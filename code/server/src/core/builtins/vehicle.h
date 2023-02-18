#pragma once

#include "integrations/server/scripting/builtins/node/entity.h"
#include "scripting/engines/node/engine.h"
#include "scripting/engines/node/sdk.h"
#include "shared/modules/vehicle_sync.hpp"
#include "core/modules/vehicle.h"
#include "shared/game_rpc/set_vehicledata.h"

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

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            if (!rootModule) {
                return;
            }

            v8pp::class_<Vehicle> cls(isolate);
            cls.inherit<Framework::Integrations::Scripting::Entity>();
            cls.function("lock", &Vehicle::Lock);
            cls.function("getLockState", &Vehicle::GetLockState);
            rootModule->class_("Vehicle", cls);
        }
    };
} // namespace MafiaMP::Scripting
