#pragma once

#include "integrations/server/scripting/builtins/node/entity.h"
#include "scripting/engines/node/engine.h"
#include "scripting/engines/node/sdk.h"
#include "shared/modules/human_sync.hpp"

#include "shared/game_rpc/add_weapon.h"
#include "shared/game_rpc/human/human_setprops.h"
#include "shared/rpc/chat_message.h"

#include "scripting/module.h"

#include "vehicle.h"

namespace MafiaMP::Scripting {
    class Human final: public Framework::Integrations::Scripting::Entity {
      public:
        Human(flecs::entity_t ent): Entity(ent) {}

        static v8::Local<v8::Object> WrapHuman(Framework::Scripting::Engines::Node::Engine *engine, flecs::entity e) {
            return v8pp::class_<Scripting::Human>::create_object(engine->GetIsolate(), e.id());
        }

        std::string ToString() const override {
            std::ostringstream ss;
            ss << "Human{ id: " << _ent.id() << " }";
            return ss.str();
        }

        void Destroy(v8::Isolate *isolate) {
            isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "Human object can not be destroyed!").ToLocalChecked()));
        }

        void AddWeapon(int weaponId, int ammo) {
            FW_SEND_SERVER_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::AddWeapon, _ent, weaponId, ammo);
        }

        void SendChat(std::string message) {
            const auto str = _ent.get<Framework::World::Modules::Base::Streamer>();
            FW_SEND_COMPONENT_RPC_TO(Shared::RPC::ChatMessage, SLNet::RakNetGUID(str->guid), message);
        }

        v8::Local<v8::Value> GetVehicle() const {
            const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
            const auto carEnt     = flecs::entity(_ent.world(), updateData->carPassenger.carId);
            if (carEnt.is_valid() && carEnt.is_alive()) {
                return v8pp::class_<Vehicle>::create_object(v8::Isolate::GetCurrent(), carEnt.id());
            }
            return v8::Undefined(v8::Isolate::GetCurrent());
        }

        int GetVehicleSeat() const {
            const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
            const auto carEnt     = flecs::entity(_ent.world(), updateData->carPassenger.carId);
            if (carEnt.is_valid() && carEnt.is_alive()) {
                return updateData->carPassenger.seatId;
            }
            return -1;
        }

        static void SendChatToAll(std::string message) {
            FW_SEND_COMPONENT_RPC(Shared::RPC::ChatMessage, message);
        }

        void SetHealth(float health) {
            auto h            = _ent.get_mut<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
            h->_healthPercent = health;
            MafiaMP::Shared::RPC::HumanSetProps msg {};
            msg.health = health;
            FW_SEND_SERVER_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::HumanSetProps, _ent, msg);
        }

        float GetHealth() const {
            auto h = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
            return h->_healthPercent;
        }

        static void EventPlayerDied(flecs::entity e) {
            const auto engine = MafiaMP::Server::GetNodeEngine();
            V8_RESOURCE_LOCK(engine);
            auto playerObj = WrapHuman(engine, e);
            engine->InvokeEvent("playerDied", playerObj);
        }

        static void EventPlayerConnected(flecs::entity e) {
            const auto engine = MafiaMP::Server::GetNodeEngine();
            V8_RESOURCE_LOCK(engine);
            auto playerObj = WrapHuman(engine, e);
            engine->InvokeEvent("playerConnected", playerObj);
        }

        static void EventPlayerDisconnected(flecs::entity e) {
            const auto engine = MafiaMP::Server::GetNodeEngine();
            V8_RESOURCE_LOCK(engine);
            auto playerObj = WrapHuman(engine, e);
            engine->InvokeEvent("playerDisconnected", playerObj);
        }

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule) {
            if (!rootModule) {
                return;
            }

            v8pp::class_<Human> cls(isolate);
            cls.inherit<Framework::Integrations::Scripting::Entity>();
            cls.function("destroy", &Human::Destroy);
            cls.function("addWeapon", &Human::AddWeapon);
            cls.function("setHealth", &Human::SetHealth);
            cls.function("getHealth", &Human::GetHealth);
            cls.function("getVehicle", &Human::GetVehicle);
            cls.function("getVehicleSeat", &Human::GetVehicleSeat);
            cls.function("sendChat", &Human::SendChat);
            cls.function("sendChatToAll", &Human::SendChatToAll);
            rootModule->class_("Human", cls);
        }
    };
} // namespace MafiaMP::Scripting
