#pragma once

#include "scripting/engines/node/engine.h"
#include "scripting/engines/node/sdk.h"

#include "../modules/vehicle.h"
#include "vehicle.h"

namespace MafiaMP::Scripting {
    class World final {
      public:
        static v8::Local<v8::Object> CreateVehicle(v8::Isolate *isolate, std::string modelName) {
            auto e = MafiaMP::Core::Modules::Vehicle::Create(Server::_serverRef);

            auto frame       = e.get_mut<Framework::World::Modules::Base::Frame>();
            frame->modelName = modelName;

            return v8pp::class_<Scripting::Vehicle>::create_object(isolate, e.id());
          }

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            rootModule->function("createVehicle", &World::CreateVehicle);
        }
    };
} // namespace MafiaMP::Scripting
