#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include "core/server.h"

#include "player.h"

#include <integrations/server/scripting/module.h>
#include <logging/logger.h>
#include <scripting/node_engine.h>

#include <string>
#include <vector>

namespace MafiaMP::Scripting {
    // Bridges incoming chat (received and parsed by the framework) into the gamemode's reserved
    // events, carrying the mod's Player handle. Outbound chat and the JS `Chat` API live in the
    // framework (Framework::Scripting::Builtins::Chat).
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

      private:
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
