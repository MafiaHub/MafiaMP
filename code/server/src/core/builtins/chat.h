#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>
#include <v8pp/module.hpp>

#include "core/server.h"

#include "shared/rpc/ids.h"

#include "player.h"

#include <core_modules.h>
#include <integrations/server/scripting/module.h>
#include <logging/logger.h>
#include <networking/network_peer.h>
#include <scripting/node_engine.h>

#include <mafianet/BitStream.h>
#include <mafianet/string.h>

#include <string>
#include <vector>

namespace MafiaMP::Scripting {
    class Chat final {
      public:
        static void EventChatMessage(uint64_t playerId, std::string message) {
            Framework::Logging::GetLogger("Scripting")->debug("Chat message from {}: {}", playerId, message);
            EmitChat([&](v8::Isolate *isolate, std::vector<v8::Local<v8::Value>> &args) {
                args.push_back(v8pp::class_<Player>::create_object(isolate, playerId));
                args.push_back(v8pp::to_v8(isolate, message));
            }, "chatMessage");
        }

        static void EventChatCommand(uint64_t playerId, std::string message, std::string command, std::vector<std::string> commandArgs) {
            Framework::Logging::GetLogger("Scripting")->debug("Chat command from {}: /{} ({})", playerId, command, message);
            EmitChat([&](v8::Isolate *isolate, std::vector<v8::Local<v8::Value>> &args) {
                auto ctx                       = isolate->GetCurrentContext();
                v8::Local<v8::Array> argsArray = v8::Array::New(isolate, static_cast<int>(commandArgs.size()));
                for (size_t i = 0; i < commandArgs.size(); ++i) {
                    argsArray->Set(ctx, static_cast<uint32_t>(i), v8pp::to_v8(isolate, commandArgs[i])).Check();
                }
                args.push_back(v8pp::class_<Player>::create_object(isolate, playerId));
                args.push_back(v8pp::to_v8(isolate, message));
                args.push_back(v8pp::to_v8(isolate, command));
                args.push_back(argsArray);
            }, "chatCommand");
        }

        static void SendToAll(std::string message) {
            Send(message, MafiaNet::UNASSIGNED_RAKNET_GUID, true);
        }

        static void SendToPlayer(Player *player, std::string message) {
            if (!player) {
                return;
            }
            auto *human = player->ResolveHuman();
            if (!human) {
                return;
            }
            Send(message, MafiaNet::RakNetGUID(human->ownerGUID), false);
        }

        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
            if (!isolate || global.IsEmpty()) {
                return;
            }
            v8pp::module chatModule(isolate);
            chatModule.function("sendToAll", &Chat::SendToAll);
            chatModule.function("sendToPlayer", &Chat::SendToPlayer);

            auto ctx = isolate->GetCurrentContext();
            global->Set(ctx, v8pp::to_v8(isolate, "Chat"), chatModule.new_instance()).Check();
        }

      private:
        // Wire: <text>
        static void Send(const std::string &message, MafiaNet::RakNetGUID target, bool broadcast) {
            auto *net = Framework::CoreModules::GetNetworkPeer();
            if (!net) {
                return;
            }
            MafiaNet::BitStream bs;
            bs.Write(MafiaNet::RakString(message.c_str()));
            net->GetRPC()->Signal(Shared::RPC::kChatMessage, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, target, broadcast, false);
        }

        template <typename ArgsBuilder>
        static void EmitChat(ArgsBuilder &&buildArgs, const std::string &eventName) {
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

            std::vector<v8::Local<v8::Value>> args;
            buildArgs(isolate, args);
            resourceManager->GetEvents().EmitReserved(isolate, context, eventName, args);
        }
    };
} // namespace MafiaMP::Scripting
