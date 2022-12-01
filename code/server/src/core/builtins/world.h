#pragma once

#include "scripting/engines/node/engine.h"
#include "scripting/engines/node/sdk.h"

#include "../modules/vehicle.h"
#include "vehicle.h"

namespace MafiaMP::Scripting {
    class World final {
      public:
          static Scripting::Vehicle CreateVehicle(std::string modelName) {
            auto e = MafiaMP::Core::Modules::Vehicle::Create(Server::_serverRef);

            auto frame       = e.get_mut<Framework::World::Modules::Base::Frame>();
            frame->modelName = modelName;
            return Scripting::Vehicle(e.id());
          }

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            auto mod = v8pp::module(isolate);
            mod.function("createVehicle", &World::CreateVehicle);

            rootModule->submodule("world", mod);
        }
    };
} // namespace MafiaMP::Scripting
