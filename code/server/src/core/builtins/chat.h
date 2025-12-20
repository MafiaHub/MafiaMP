#pragma once

#include <sol/sol.hpp>

#include "core/server.h"

#include "shared/rpc/chat_message.h"

#include "player.h"

#include <scripting/resource/resource_manager.h>

namespace MafiaMP::Scripting {
    class Chat final {
      public:
        static void EventChatMessage(flecs::entity e, std::string message) {
            const auto resourceManager = Framework::CoreModules::GetResourceManager();
            if (resourceManager) {
                resourceManager->InvokeGlobalEvent("onChatMessage", Player(e), message);
            }
        }

        static void EventChatCommand(flecs::entity e, std::string message, std::string command, std::vector<std::string> args) {
            const auto resourceManager = Framework::CoreModules::GetResourceManager();
            if (resourceManager) {
                resourceManager->InvokeGlobalEvent("onChatCommand", Player(e), message, command, args);
            }
        }

        static void SendToAll(std::string message) {
            FW_SEND_COMPONENT_RPC(Shared::RPC::ChatMessage, message);
        }

        static void SendToPlayer(Player *player, std::string message) {
            if (player) {
                const auto ent = player->GetHandle();
                const auto streamer = ent.get<Framework::World::Modules::Base::Streamer>();

                if (!streamer) {
                    return;
                }

                FW_SEND_COMPONENT_RPC_TO(Shared::RPC::ChatMessage, SLNet::RakNetGUID(streamer->guid), message);
            }
        }

        static void Register(sol::state *luaEngine) {
            if (!luaEngine) {
                return;
            }

            sol::usertype<Chat> cls = luaEngine->new_usertype<Chat>("Chat");
            cls["sendToAll"]        = &Chat::SendToAll;
            cls["sendToPlayer"]     = &Chat::SendToPlayer;
        }
    };
} // namespace MafiaMP::Scripting
