#pragma once

#include "scripting/engines/node/engine.h"
#include "scripting/engines/node/sdk.h"

#include "../modules/environment.h"
#include "../modules/vehicle.h"

#include "../../shared/rpc/environment.h"

#include "shared/rpc/chat_message.h"

#include "vehicle.h"
#include "player.h"

#include "core_modules.h"

namespace MafiaMP::Scripting {
    class World final {
      public:
        static v8::Local<v8::Object> WrapVehicle(v8::Isolate *isolate, flecs::entity e) {
            return v8pp::class_<Scripting::Vehicle>::create_object(isolate, e.id());
        }

        static v8::Local<v8::Object> CreateVehicle(v8::Isolate *isolate, std::string modelName) {
            auto e = MafiaMP::Core::Modules::Vehicle::Create(Server::_serverRef);

            auto frame       = e.get_mut<Framework::World::Modules::Base::Frame>();
            frame->modelName = modelName;

            return WrapVehicle(isolate, e);
        }

        static void SetWeather(std::string weatherSetName) {
            auto world = Framework::CoreModules::GetWorldEngine()->GetWorld();

            auto weather             = world->get_mut<Core::Modules::Environment::Weather>();
            weather->_weatherSetName = weatherSetName;
            FW_SEND_COMPONENT_RPC(MafiaMP::Shared::RPC::SetEnvironment, SLNet::RakString(weather->_weatherSetName.c_str()), {});
        }

        static void SetDayTimeHours(float dayTimeHours) {
            auto world = Framework::CoreModules::GetWorldEngine()->GetWorld();

            auto weather           = world->get_mut<Core::Modules::Environment::Weather>();
            weather->_dayTimeHours = dayTimeHours;
            FW_SEND_COMPONENT_RPC(MafiaMP::Shared::RPC::SetEnvironment, {}, weather->_dayTimeHours);
        }

        static void SendChatMessage(std::string message, Human *human) {
            if (human) {
                const auto ent = human->GetHandle();
                const auto str   = ent.get<Framework::World::Modules::Base::Streamer>();
                if (!str)
                    return;
                FW_SEND_COMPONENT_RPC_TO(Shared::RPC::ChatMessage, SLNet::RakNetGUID(str->guid), message);
            }
        }

        static void BroadcastMessage(std::string message) {
            FW_SEND_COMPONENT_RPC(Shared::RPC::ChatMessage, message);
        }

        static void OnChatMessage(flecs::entity e, std::string message) {
            // TODO: fix me
            /*auto nodeResource = reinterpret_cast<Framework::Scripting::Engines::Node::Resource *>(resource);
            nodeResource->InvokeEvent("chatMessage", Human::WrapHuman(nodeResource, e), message);*/
        }

        static void OnChatCommand(flecs::entity e, std::string message, std::string command, std::vector<std::string> args) {
            //TODO: fix me
            /*auto nodeResource = reinterpret_cast<Framework::Scripting::Engines::Node::Resource *>(resource);
            nodeResource->InvokeEvent("chatCommand", Human::WrapHuman(nodeResource, e), message, command, args);*/
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

            rootModule->function("sendChatMessage", &World::SendChatMessage);
            rootModule->function("broadcastMessage", &World::BroadcastMessage);
        }
    };
} // namespace MafiaMP::Scripting
