#pragma once

#include "integrations/server/scripting/builtins/node/entity.h"
#include "scripting/engines/node/engine.h"
#include "scripting/engines/node/sdk.h"
#include "shared/modules/human_sync.hpp"

namespace MafiaMP::Scripting {
    class Human final: public Framework::Integrations::Scripting::Entity {
      public:
        Human(flecs::entity_t ent): Entity(ent) {}

        std::string ToString() const override {
            std::ostringstream ss;
            ss << "Vehicle{ id: " << _ent.id() << " }";
            return ss.str();
        }

        void Destruct() {
            // TODO raise exception
        }

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            if (!rootModule) {
                return;
            }

            v8pp::class_<Human> cls(isolate);
            cls.inherit<Framework::Integrations::Scripting::Entity>();
            cls.function("destruct", &Human::Destruct);
            rootModule->class_("Human", cls);
        }
    };
} // namespace MafiaMP::Scripting
