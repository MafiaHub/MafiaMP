#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>
#include <v8pp/module.hpp>

#include "core/server.h"

#include "shared/rpc/chat_message.h"

#include "player.h"

#include <integrations/server/scripting/module.h>
#include <logging/logger.h>
#include <scripting/node_engine.h>

#include <memory>

namespace MafiaMP::Scripting {
    class Chat final {
      public:
        static void EventChatMessage(flecs::entity e, std::string message) {
            Framework::Logging::GetLogger("Scripting")->debug("Chat message from {}: {}", e.id(), message);

            auto server = MafiaMP::Server::_serverRef;
            if (!server)
                return;

            auto scriptingModule = server->GetScriptingModule();
            if (!scriptingModule)
                return;

            auto *engine          = scriptingModule->GetEngine();
            auto *resourceManager = scriptingModule->GetResourceManager();
            if (!engine || !resourceManager || !engine->IsInitialized())
                return;

            v8::Isolate *isolate = engine->GetIsolate();
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);
            v8::Local<v8::Context> context = engine->GetContext();
            v8::Context::Scope contextScope(context);

            auto playerObj = Player::GetClass(isolate).import_external(isolate, new Player(e));

            std::vector<v8::Local<v8::Value>> args;
            args.push_back(playerObj);
            args.push_back(v8pp::to_v8(isolate, message));

            resourceManager->GetEvents().EmitReserved(isolate, context, "chatMessage", args);
        }

        static void EventChatCommand(flecs::entity e, std::string message, std::string command, std::vector<std::string> commandArgs) {
            Framework::Logging::GetLogger("Scripting")->debug("Chat command from {}: /{} ({})", e.id(), command, message);

            auto server = MafiaMP::Server::_serverRef;
            if (!server)
                return;

            auto scriptingModule = server->GetScriptingModule();
            if (!scriptingModule)
                return;

            auto *engine          = scriptingModule->GetEngine();
            auto *resourceManager = scriptingModule->GetResourceManager();
            if (!engine || !resourceManager || !engine->IsInitialized())
                return;

            v8::Isolate *isolate = engine->GetIsolate();
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);
            v8::Local<v8::Context> context = engine->GetContext();
            v8::Context::Scope contextScope(context);

            auto playerObj = Player::GetClass(isolate).import_external(isolate, new Player(e));

            // Convert command args to JS array
            v8::Local<v8::Array> argsArray = v8::Array::New(isolate, static_cast<int>(commandArgs.size()));
            for (size_t i = 0; i < commandArgs.size(); ++i) {
                argsArray->Set(context, static_cast<uint32_t>(i), v8pp::to_v8(isolate, commandArgs[i])).Check();
            }

            std::vector<v8::Local<v8::Value>> args;
            args.push_back(playerObj);
            args.push_back(v8pp::to_v8(isolate, message));
            args.push_back(v8pp::to_v8(isolate, command));
            args.push_back(argsArray);

            resourceManager->GetEvents().EmitReserved(isolate, context, "chatCommand", args);
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
