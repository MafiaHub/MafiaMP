#include "player.h"

#include "vehicle.h"

#include "shared/modules/human_sync.hpp"
#include "shared/game_rpc/add_weapon.h"
#include "shared/game_rpc/human/human_setprops.h"
#include "shared/rpc/chat_message.h"

namespace MafiaMP::Scripting {
    /*v8::Local<v8::Object> Human::WrapHuman(Framework::Scripting::Engine *engine, flecs::entity e) {
        return v8pp::class_<Scripting::Human>::create_object(engine->GetIsolate(), e.id());
    }*/

    std::string Human::ToString() const {
        std::ostringstream ss;
        ss << "Human{ id: " << _ent.id() << " }";
        return ss.str();
    }

    void Human::Destroy() {
        // TODO: fix this
        // isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "Human object can not be destroyed!").ToLocalChecked()));
    }

    void Human::AddWeapon(int weaponId, int ammo) {
        FW_SEND_SERVER_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::AddWeapon, _ent, weaponId, ammo);
    }

    void Human::SendChat(std::string message) {
        const auto str = _ent.get<Framework::World::Modules::Base::Streamer>();
        FW_SEND_COMPONENT_RPC_TO(Shared::RPC::ChatMessage, SLNet::RakNetGUID(str->guid), message);
    }

    /*v8::Local<v8::Value> Human::GetVehicle() const {
        const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        const auto carEnt     = flecs::entity(_ent.world(), updateData->carPassenger.carId);
        if (carEnt.is_valid() && carEnt.is_alive()) {
            return v8pp::class_<Vehicle>::create_object(v8::Isolate::GetCurrent(), carEnt.id());
        }
        return v8::Undefined(v8::Isolate::GetCurrent());
    }*/

    int Human::GetVehicleSeat() const {
        const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        const auto carEnt     = flecs::entity(_ent.world(), updateData->carPassenger.carId);
        if (carEnt.is_valid() && carEnt.is_alive()) {
            return updateData->carPassenger.seatId;
        }
        return -1;
    }

    void Human::SendChatToAll(std::string message) {
        FW_SEND_COMPONENT_RPC(Shared::RPC::ChatMessage, message);
    }

    void Human::SetHealth(float health) {
        auto h            = _ent.get_mut<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        h->_healthPercent = health;
        MafiaMP::Shared::RPC::HumanSetProps msg {};
        msg.health = health;
        FW_SEND_SERVER_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::HumanSetProps, _ent, msg);
    }

    float Human::GetHealth() const {
        auto h = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        return h->_healthPercent;
    }

    void Human::EventPlayerDied(flecs::entity e) {
        const auto engine = MafiaMP::Server::GetScriptingEngine();
        /*auto playerObj = WrapHuman(engine, e);
        engine->InvokeEvent("playerDied", playerObj);*/
    }

    void Human::EventPlayerConnected(flecs::entity e) {
        const auto engine = MafiaMP::Server::GetScriptingEngine();
        /*auto playerObj = WrapHuman(engine, e);
        engine->InvokeEvent("playerConnected", playerObj);*/
    }

    void Human::EventPlayerDisconnected(flecs::entity e) {
        const auto engine = MafiaMP::Server::GetScriptingEngine();
        /*auto playerObj = WrapHuman(engine, e);
        engine->InvokeEvent("playerDisconnected", playerObj);*/
    }

    void Human::Register(sol::state &luaEngine) {
        sol::usertype<Human> cls = luaEngine.new_usertype<Human>("Human", sol::constructors<Human(uint64_t)>(), sol::base_classes, sol::bases<Entity>());
        cls["destroy"] = &Human::Destroy;
        cls["addWeapon"] = &Human::AddWeapon;
        cls["setHealth"] = &Human::SetHealth;
        cls["getHealth"] = &Human::GetHealth;
        //cls["getVehicle"] = &Human::GetVehicle;
        cls["getVehicleSeat"] = &Human::GetVehicleSeat;
        cls["sendChat"] = &Human::SendChat;
        cls["sendChatToAll"] = &Human::SendChatToAll;
    }
}
