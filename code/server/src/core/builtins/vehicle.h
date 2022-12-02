#pragma once

#include "integrations/server/scripting/builtins/node/entity.h"
#include "scripting/engines/node/engine.h"
#include "scripting/engines/node/sdk.h"
#include "shared/modules/vehicle_sync.hpp"
#include "shared/rpc/set_vehicledata.h"

namespace MafiaMP::Scripting {
    class Vehicle final: public Framework::Integrations::Scripting::Entity {
      public:
        Vehicle(flecs::entity_t ent): Entity(ent) {}

        std::string ToString() const override {
            std::ostringstream ss;
            ss << "Vehicle{ id: " << _ent.id() << " }";
            return ss.str();
        }

        void SetHorn(bool state) {
            auto data = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            data->horn = state;

            FW_SEND_COMPONENT_GAME_RPC(Shared::RPC::SetVehicleData, _ent, *data);
        }

        bool GetHorn() {
            const auto data = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
            return data->horn;
        }

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            if (!rootModule) {
                return;
            }

            v8pp::class_<Vehicle> cls(isolate);
            cls.inherit<Framework::Integrations::Scripting::Entity>();
            cls.function("setHorn", &Vehicle::SetHorn);
            cls.function("getHorn", &Vehicle::GetHorn);
            rootModule->class_("Vehicle", cls);
        }
    };
} // namespace MafiaMP::Scripting
