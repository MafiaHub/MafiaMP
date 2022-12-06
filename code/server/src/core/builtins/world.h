#pragma once

#include "scripting/engines/node/engine.h"
#include "scripting/engines/node/sdk.h"

#include "../modules/environment.h"
#include "../modules/vehicle.h"

#include "../../shared/rpc/environment.h"

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

        static void SetWeather(std::string weatherSetName) {
            auto world = Framework::World::Engine::_worldRef;

            auto weather             = world->get_mut<Core::Modules::Environment::Weather>();
            weather->_weatherSetName = weatherSetName;
            FW_SEND_COMPONENT_RPC(MafiaMP::Shared::RPC::SetEnvironment, SLNet::RakString(weather->_weatherSetName.c_str()), {});
        }

        static void SetDayTimeHours(float dayTimeHours) {
            auto world = Framework::World::Engine::_worldRef;

            auto weather           = world->get_mut<Core::Modules::Environment::Weather>();
            weather->_dayTimeHours = dayTimeHours;
            FW_SEND_COMPONENT_RPC(MafiaMP::Shared::RPC::SetEnvironment, {}, weather->_dayTimeHours);
        }

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            // Create the environment namespace
            v8pp::module environment(isolate);
            environment.function("setWeather", &World::SetWeather);
            environment.function("setDayTimeHours", &World::SetDayTimeHours);
            rootModule->submodule("Environment", environment);

            // Create the vehicle namespace
            v8pp::module world(isolate);
            world.function("createVehicle", &World::CreateVehicle);
            rootModule->submodule("World", world);
        }
    };
} // namespace MafiaMP::Scripting
