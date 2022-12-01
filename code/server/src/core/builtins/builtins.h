#pragma once

#include "scripting/engines/node/engine.h"
#include "scripting/engines/node/sdk.h"

#include "world.h"
#include "vehicle.h"

namespace MafiaMP::Scripting {
    class Builtins final {
      public:
        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            Scripting::World::Register(isolate, rootModule);
            Scripting::Vehicle::Register(isolate, rootModule);
        }
    };
}
