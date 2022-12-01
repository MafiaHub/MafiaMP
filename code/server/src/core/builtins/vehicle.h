#pragma once

#include "scripting/engines/node/engine.h"
#include "scripting/engines/node/sdk.h"
#include "integrations/server/scripting/builtins/node/entity.h"

namespace MafiaMP::Scripting {
    class Vehicle final : public Framework::Integrations::Scripting::Entity {
      public:
        Vehicle(flecs::entity_t ent): Entity(ent) {}

        std::string ToString() const override {
            std::ostringstream ss;
            ss << "Vehicle{ id: " << _ent.id() << " }";
            return ss.str();
        }

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            if (!rootModule) {
                return;
            }

            v8pp::class_<Vehicle> cls(isolate);

            rootModule->class_("Vehicle", cls);
        }
    };
} // namespace MafiaMP::Scripting
