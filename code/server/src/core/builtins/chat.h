#pragma once

#include <sol/sol.hpp>

#include "scripting/server_engine.h"

#include "shared/rpc/chat_message.h"

#include "player.h"

#include "core_modules.h"

namespace MafiaMP::Scripting {
    class Chat final {
      public:
        static void SendToPlayer(Human *human, std::string message) {
            if (human) {
                const auto ent = human->GetHandle();
                const auto str   = ent.get<Framework::World::Modules::Base::Streamer>();

                if (!str)
                    return;

                FW_SEND_COMPONENT_RPC_TO(Shared::RPC::ChatMessage, SLNet::RakNetGUID(str->guid), message);
            }
        }

        static void SendToAll(std::string message) {
            FW_SEND_COMPONENT_RPC(Shared::RPC::ChatMessage, message);
        }

        static void EventChatMessage(flecs::entity e, std::string message) {
            const auto engine = MafiaMP::Server::GetScriptingEngine();
            engine->InvokeEvent("onChatMessage", Human(e), message);
        }

        static void EventChatCommand(flecs::entity e, std::string message, std::string command, std::vector<std::string> args) {
            const auto engine = MafiaMP::Server::GetScriptingEngine();
            engine->InvokeEvent("onChatCommand", Human(e), message, command, args);
        }

        static void Register(sol::state &luaEngine) {
            sol::usertype<Chat> cls = luaEngine.new_usertype<Chat>("Chat");
            cls["sendToPlayer"] = &Chat::SendToPlayer;
            cls["sendToAll"]    = &Chat::SendToAll;
        }
    };
} // namespace MafiaMP::Scripting
