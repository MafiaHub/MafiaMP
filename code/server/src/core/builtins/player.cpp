#include "player.h"

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
        auto playerObj = WrapPlayer(engine, e);
        engine->InvokeEvent("playerConnected", playerObj);
    }

    void Player::EventPlayerDisconnected(flecs::entity e) {
        const auto engine = MafiaMP::Server::GetNodeEngine();
        V8_RESOURCE_LOCK(engine);
        auto playerObj = WrapPlayer(engine, e);
        engine->InvokeEvent("playerDisconnected", playerObj);
    }

    void Player::EventPlayerDied(flecs::entity e) {
        const auto engine = MafiaMP::Server::GetNodeEngine();
        V8_RESOURCE_LOCK(engine);
        auto playerObj = WrapPlayer(engine, e);
        engine->InvokeEvent("playerDied", playerObj);
    }

    void Player::EventPlayerVehicleEnter(flecs::entity player, flecs::entity vehicle, int seatIndex) {
        const auto engine = MafiaMP::Server::GetNodeEngine();
        V8_RESOURCE_LOCK(engine);

        auto vehicleObj = Vehicle::WrapVehicle(engine, vehicle);
        auto playerObj  = WrapPlayer(engine, player);

        engine->InvokeEvent("playerVehicleEnter", playerObj, vehicleObj, seatIndex);
    }

    void Player::EventPlayerVehicleLeave(flecs::entity player, flecs::entity vehicle) {
        const auto engine = MafiaMP::Server::GetNodeEngine();
        V8_RESOURCE_LOCK(engine)

        auto vehicleObj = Vehicle::WrapVehicle(engine, vehicle);
        auto playerObj  = WrapPlayer(engine, player);

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

    v8::Local<v8::Object> Player::WrapPlayer(Framework::Scripting::Engines::Node::Engine *engine, flecs::entity e) {
        return v8pp::class_<Scripting::Player>::create_object(engine->GetIsolate(), e.id());
    }
} // namespace MafiaMP::Scripting
