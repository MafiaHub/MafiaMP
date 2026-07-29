#pragma once

#include <v8.h>

#include <scripting/builtins/entity.h>

#include "human.h"
#include "player.h"
#include "vehicle.h"
#include "world.h"

namespace MafiaMP::Scripting {
    class Builtins final {
      public:
        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
            if (!isolate || global.IsEmpty()) {
                return;
            }

            // Register entity classes at the global root
            Framework::Scripting::Builtins::Entity::Register(isolate, global);
            Scripting::Human::Register(isolate, global);
            Scripting::Player::Register(isolate, global);
            Scripting::Vehicle::Register(isolate, global);

            // Register module singletons on global for direct access (World). The Chat API is
            // registered by the framework.
            Scripting::World::Register(isolate, global);
        }
    };
} // namespace MafiaMP::Scripting
