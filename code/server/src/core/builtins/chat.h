#pragma once

#include "core/server.h"

#include "shared/rpc/chat_message.h"

#include "player.h"

namespace MafiaMP::Scripting {
    class Chat final {
      public:
        static void EventChatCommand(flecs::entity e, std::string message, std::string command, std::vector<std::string> args) {
            const auto engine = MafiaMP::Server::GetNodeEngine();
            V8_RESOURCE_LOCK(engine);
            engine->InvokeEvent("chatCommand", Player::WrapPlayer(engine->GetIsolate(), e), message, command, args);
        }

        static void EventChatMessage(flecs::entity e, std::string message) {
            const auto engine = MafiaMP::Server::GetNodeEngine();
            V8_RESOURCE_LOCK(engine);
            engine->InvokeEvent("chatMessage", Player::WrapPlayer(engine->GetIsolate(), e), message);
        }

        static void SendToAll(std::string message) {
            FW_SEND_COMPONENT_RPC(Shared::RPC::ChatMessage, message);
        }

        static void SendToPlayer(Human *human, std::string message) {
            if (human) {
                const auto ent = human->GetHandle();
                const auto str = ent.get<Framework::World::Modules::Base::Streamer>();

                if (!str)
                    return;

                FW_SEND_COMPONENT_RPC_TO(Shared::RPC::ChatMessage, SLNet::RakNetGUID(str->guid), message);
            }
        }

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            v8pp::module chat(isolate);

            chat.function("sendToAll", &Chat::SendToAll);
            chat.function("sendToPlayer", &Chat::SendToPlayer);

            rootModule->submodule("Chat", chat);
        }
    };
} // namespace MafiaMP::Scripting
