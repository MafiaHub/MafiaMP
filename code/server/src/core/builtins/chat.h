#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>
#include <v8pp/module.hpp>

#include "core/server.h"

#include "shared/rpc/chat_message.h"

#include "player.h"

#include <logging/logger.h>

#include <memory>

namespace MafiaMP::Scripting {
    class Chat final {
      public:
        static void EventChatMessage(flecs::entity e, std::string message) {
            // TODO: Implement event dispatch via Framework::Scripting::Events::EmitGlobal()
            Framework::Logging::GetLogger("Scripting")->debug("Chat message from {}: {}", e.id(), message);
        }

        static void EventChatCommand(flecs::entity e, std::string message, std::string command, std::vector<std::string> args) {
            // TODO: Implement event dispatch via Framework::Scripting::Events::EmitGlobal()
            Framework::Logging::GetLogger("Scripting")->debug("Chat command from {}: /{} ({})", e.id(), command, message);
        }

        static void SendToAll(std::string message) {
            FW_SEND_COMPONENT_RPC(Shared::RPC::ChatMessage, message);
        }

        static void SendToPlayer(Player *player, std::string message) {
            if (player) {
                const auto ent      = player->GetHandle();
                const auto streamer = ent.get<Framework::World::Modules::Base::Streamer>();

                if (!streamer) {
                    return;
                }

                FW_SEND_COMPONENT_RPC_TO(Shared::RPC::ChatMessage, SLNet::RakNetGUID(streamer->guid), message);
            }
        }

        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
            if (!isolate || global.IsEmpty()) {
                return;
            }

            // Create Chat module object with static methods
            v8pp::module chatModule(isolate);
            chatModule.set("sendToAll", &Chat::SendToAll);
            chatModule.set("sendToPlayer", &Chat::SendToPlayer);

            auto ctx = isolate->GetCurrentContext();
            global->Set(ctx, v8pp::to_v8(isolate, "Chat"), chatModule.new_instance()).Check();
        }
    };
} // namespace MafiaMP::Scripting
