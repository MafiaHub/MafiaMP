#pragma once

#include "scripting/engines/node/engine.h"
#include "scripting/engines/node/sdk.h"

#include "shared/rpc/chat_message.h"

#include "player.h"

#include "core_modules.h"

namespace MafiaMP::Scripting {
    class Chat final {
      public:
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

        static void EventChatMessage(flecs::entity e, std::string message) {
            auto engine = reinterpret_cast<Framework::Scripting::Engines::Node::Engine*>(Framework::CoreModules::GetScriptingModule()->GetEngine());
            engine->InvokeEvent("chatMessage", Human::WrapHuman(engine, e), message);
        }

        static void EventChatCommand(flecs::entity e, std::string message, std::string command, std::vector<std::string> args) {
            auto engine = reinterpret_cast<Framework::Scripting::Engines::Node::Engine*>(Framework::CoreModules::GetScriptingModule()->GetEngine());
            engine->InvokeEvent("chatCommand", Human::WrapHuman(engine, e), message, command, args);
        }

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            rootModule->function("sendChatMessage", &Chat::SendChatMessage);
            rootModule->function("broadcastMessage", &Chat::BroadcastMessage);
        }
    };
} // namespace MafiaMP::Scripting
