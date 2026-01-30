#include "player.h"

#include "core/server.h"

#include "shared/rpc/chat_message.h"

#include <integrations/server/scripting/module.h>
#include <logging/logger.h>
#include <scripting/node_engine.h>
#include <world/modules/base.hpp>

namespace MafiaMP::Scripting {

std::unique_ptr<v8pp::class_<Player>> Player::_class;

namespace {
    // Helper to emit player events with a Player JS object argument
    void EmitPlayerEvent(flecs::entity e, const std::string &eventName) {
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

        auto playerObj = v8pp::class_<Player>::create_object(isolate, e);

        std::vector<v8::Local<v8::Value>> args;
        args.push_back(playerObj);

        resourceManager->GetEvents().EmitReserved(isolate, context, eventName, args);
    }
} // namespace

void Player::EventPlayerConnected(flecs::entity e) {
    Framework::Logging::GetLogger("Scripting")->debug("Player connected: {}", e.id());
    EmitPlayerEvent(e, "playerConnect");
}

void Player::EventPlayerDisconnected(flecs::entity e) {
    Framework::Logging::GetLogger("Scripting")->debug("Player disconnected: {}", e.id());
    EmitPlayerEvent(e, "playerDisconnect");
}

void Player::EventPlayerDied(flecs::entity e) {
    Framework::Logging::GetLogger("Scripting")->debug("Player died: {}", e.id());
    EmitPlayerEvent(e, "playerDied");
}

std::string Player::ToString() const {
    std::ostringstream ss;
    ss << "Player{ id: " << _ent.id() << " }";
    return ss.str();
}

void Player::Destroy() {
    // Nothing should happen here, as the player entity is destroyed by the game and network systems
}

void Player::SendChat(std::string message) {
    const auto streamer = _ent.get<Framework::World::Modules::Base::Streamer>();
    if (streamer) {
        FW_SEND_COMPONENT_RPC_TO(Shared::RPC::ChatMessage, SLNet::RakNetGUID(streamer->guid), message);
    }
}

v8pp::class_<Player> &Player::GetClass(v8::Isolate *isolate) {
    if (!_class) {
        _class = std::make_unique<v8pp::class_<Player>>(isolate);
        _class->auto_wrap_objects(true);
        _class->inherit<Human>()
            .ctor<flecs::entity_t>()
            .set("toString", &Player::ToString)
            .set("destroy", &Player::Destroy)
            .set("sendChat", &Player::SendChat);
    }
    return *_class;
}

void Player::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
    v8pp::class_<Player> &cls = GetClass(isolate);
    auto ctx                  = isolate->GetCurrentContext();
    global->Set(ctx, v8pp::to_v8(isolate, "Player"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
}

} // namespace MafiaMP::Scripting
