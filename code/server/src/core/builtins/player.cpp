#include "player.h"

#include "core/server.h"

#include <core_modules.h>
#include <integrations/server/scripting/module.h>
#include <logging/logger.h>
#include <networking/network_peer.h>
#include <networking/rpc/chat_message.h>
#include <scripting/node_engine.h>

namespace MafiaMP::Scripting {

std::unique_ptr<v8pp::class_<Player>> Player::_class;

namespace {
    void EmitPlayerEvent(uint64_t networkId, const std::string &eventName) {
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

        auto playerObj = v8pp::class_<Player>::create_object(isolate, networkId);

        std::vector<v8::Local<v8::Value>> args;
        args.push_back(playerObj);

        resourceManager->GetEvents().EmitReserved(isolate, context, eventName, args);
    }
} // namespace

void Player::EventPlayerConnected(uint64_t networkId) {
    Framework::Logging::GetLogger("Scripting")->debug("Player connected: {}", networkId);
    EmitPlayerEvent(networkId, "playerConnect");
}

void Player::EventPlayerDisconnected(uint64_t networkId) {
    Framework::Logging::GetLogger("Scripting")->debug("Player disconnected: {}", networkId);
    EmitPlayerEvent(networkId, "playerDisconnect");
}

void Player::EventPlayerDied(uint64_t networkId) {
    Framework::Logging::GetLogger("Scripting")->debug("Player died: {}", networkId);
    EmitPlayerEvent(networkId, "playerDied");
}

std::string Player::ToString() const {
    std::ostringstream ss;
    ss << "Player{ id: " << _id << " }";
    return ss.str();
}

void Player::Destroy() {
    // The player entity is destroyed by the game and network systems on disconnect.
}

void Player::SendChat(std::string message) {
    // Chat's own C++ send helper is private to the builtin now, so the line is addressed here: to
    // the connection that owns this avatar, with no author, which the client renders as a notice.
    auto *handle = GetHandle();
    auto *net    = Framework::CoreModules::GetNetworkPeer();
    if (!handle || !net) {
        return;
    }
    Framework::Networking::RPC::ChatMessage payload;
    payload.text = std::move(message);
    net->SendRPC(payload, MafiaNet::ToGuid(handle->ownerGUID));
}

v8pp::class_<Player> &Player::GetClass(v8::Isolate *isolate) {
    if (!_class) {
        _class = std::make_unique<v8pp::class_<Player>>(isolate);
        _class->auto_wrap_objects(true);
        _class->inherit<Human>()
            .ctor<uint64_t>()
            .function("toString", &Player::ToString)
            .function("destroy", &Player::Destroy)
            .function("sendChat", &Player::SendChat);
    }
    return *_class;
}

void Player::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
    v8pp::class_<Player> &cls = GetClass(isolate);
    auto ctx                  = isolate->GetCurrentContext();
    global->Set(ctx, v8pp::to_v8(isolate, "Player"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
}

} // namespace MafiaMP::Scripting
