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
        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global, v8::Local<v8::Object> frameworkObj) {
            if (!isolate || global.IsEmpty() || frameworkObj.IsEmpty()) {
                return;
            }

            // Register entity classes on Framework object
            Framework::Scripting::Builtins::Entity::Register(isolate, frameworkObj);
            Scripting::Human::Register(isolate, frameworkObj);
            Scripting::Player::Register(isolate, frameworkObj);
            Scripting::Vehicle::Register(isolate, frameworkObj);

            // Register module singletons on global for direct access (World). The Chat API is
            // registered by the framework.
            Scripting::World::Register(isolate, global);
        }
    };
} // namespace MafiaMP::Scripting
