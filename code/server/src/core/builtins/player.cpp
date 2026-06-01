#include "player.h"

#include "core/server.h"

#include "shared/rpc/ids.h"

#include <core_modules.h>
#include <integrations/server/scripting/module.h>
#include <logging/logger.h>
#include <networking/network_peer.h>
#include <scripting/node_engine.h>

#include <mafianet/BitStream.h>
#include <mafianet/string.h>

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
    auto *human = ResolveHuman();
    auto *net   = Framework::CoreModules::GetNetworkPeer();
    if (!human || !net) {
        return;
    }
    // Wire: <text>
    MafiaNet::BitStream bs;
    bs.Write(MafiaNet::RakString(message.c_str()));
    net->GetRPC()->Signal(Shared::RPC::kChatMessage, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, MafiaNet::RakNetGUID(human->ownerGUID), false, false);
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
