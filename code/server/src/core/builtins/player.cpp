#include "player.h"

#include "core/server.h"

#include "vehicle.h"

#include "shared/rpc/chat_message.h"

namespace MafiaMP::Scripting {
    std::string Player::ToString() const {
        std::ostringstream ss;
        ss << "Player{ id: " << _ent.id() << " }";
        return ss.str();
    }

    void Player::EventPlayerConnected(flecs::entity e) {
        const auto engine = MafiaMP::Server::GetNodeEngine();
        V8_RESOURCE_LOCK(engine);
        auto playerObj = WrapPlayer(engine->GetIsolate(), e);
        engine->InvokeEvent("playerConnected", playerObj);
    }

    void Player::EventPlayerDisconnected(flecs::entity e) {
        const auto engine = MafiaMP::Server::GetNodeEngine();
        V8_RESOURCE_LOCK(engine);
        auto playerObj = WrapPlayer(engine->GetIsolate(), e);
        engine->InvokeEvent("playerDisconnected", playerObj);
    }

    void Player::EventPlayerDied(flecs::entity e) {
        const auto engine = MafiaMP::Server::GetNodeEngine();
        V8_RESOURCE_LOCK(engine);
        auto playerObj = WrapPlayer(engine->GetIsolate(), e);
        engine->InvokeEvent("playerDied", playerObj);
    }

    void Player::EventPlayerVehicleEnter(flecs::entity player, flecs::entity vehicle, int seatIndex) {
        const auto engine = MafiaMP::Server::GetNodeEngine();
        V8_RESOURCE_LOCK(engine);

        auto vehicleObj = Vehicle::WrapVehicle(engine->GetIsolate(), vehicle);
        auto playerObj  = WrapPlayer(engine->GetIsolate(), player);

        engine->InvokeEvent("playerVehicleEnter", playerObj, vehicleObj, seatIndex);
    }

    void Player::EventPlayerVehicleLeave(flecs::entity player, flecs::entity vehicle) {
        const auto engine = MafiaMP::Server::GetNodeEngine();
        V8_RESOURCE_LOCK(engine)

        auto vehicleObj = Vehicle::WrapVehicle(engine->GetIsolate(), vehicle);
        auto playerObj  = WrapPlayer(engine->GetIsolate(), player);

        engine->InvokeEvent("playerVehicleLeave", playerObj, vehicleObj);
    }

    void Player::SendChat(std::string message) {
        const auto str = _ent.get<Framework::World::Modules::Base::Streamer>();
        FW_SEND_COMPONENT_RPC_TO(Shared::RPC::ChatMessage, SLNet::RakNetGUID(str->guid), message);
    }

    void Player::SendChatToAll(std::string message) {
        FW_SEND_COMPONENT_RPC(Shared::RPC::ChatMessage, message);
    }

    void Player::Register(v8::Isolate *isolate, v8pp::module *rootModule) {
        if (!rootModule) {
            return;
        }

        v8pp::class_<Player> cls(isolate);
        cls.inherit<MafiaMP::Scripting::Human>();

        cls.function("sendChat", &Player::SendChat);
        cls.function("sendChatToAll", &Player::SendChatToAll);

        rootModule->class_("Player", cls);
    }

    v8::Local<v8::Object> Player::WrapPlayer(v8::Isolate *isolate, flecs::entity e) {
        return v8pp::class_<Scripting::Player>::create_object(isolate, e.id());
    }
} // namespace MafiaMP::Scripting
