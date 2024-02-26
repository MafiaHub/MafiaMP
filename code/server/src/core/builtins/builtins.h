#pragma once

#include "chat.h"
#include "human.h"
#include "player.h"
#include "vehicle.h"
#include "world.h"

namespace MafiaMP::Scripting {
    class Builtins final {
      public:
        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            Scripting::Chat::Register(isolate, rootModule);
            Scripting::Human::Register(isolate, rootModule);
            Scripting::Player::Register(isolate, rootModule);
            Scripting::Vehicle::Register(isolate, rootModule);
            Scripting::World::Register(isolate, rootModule);
        }
    };
} // namespace MafiaMP::Scripting
