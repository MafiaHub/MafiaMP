#include "human.h"

#include "core/server.h"

#include "shared/game_rpc/add_weapon.h"
#include "shared/game_rpc/human/human_setprops.h"
#include "shared/modules/human_sync.hpp"

#include "vehicle.h"

namespace MafiaMP::Scripting {
    std::string Human::ToString() const {
        std::ostringstream ss;
        ss << "Human{ id: " << _ent.id() << " }";
        return ss.str();
    }

    void Human::Destroy(v8::Isolate *isolate) {
        if (_isPlayer) {
            isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "Player object can not be destroyed!").ToLocalChecked()));
            return;
        }

        Entity::Destroy();
    }

    void Human::AddWeapon(int weaponId, int ammo) {
        FW_SEND_SERVER_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::AddWeapon, _ent, weaponId, ammo);
    }

    v8::Local<v8::Value> Human::GetVehicle() const {
        const auto engine     = MafiaMP::Server::GetNodeEngine();
        const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        const auto carEnt     = flecs::entity(_ent.world(), updateData->carPassenger.carId);
        if (carEnt.is_valid() && carEnt.is_alive()) {
            return Vehicle::WrapVehicle(engine->GetIsolate(), carEnt);
        }
        return v8::Undefined(engine->GetIsolate());
    }

    int Human::GetVehicleSeat() const {
        const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        const auto carEnt     = flecs::entity(_ent.world(), updateData->carPassenger.carId);
        if (carEnt.is_valid() && carEnt.is_alive()) {
            return updateData->carPassenger.seatId;
        }
        return -1;
    }

    float Human::GetHealth() const {
        auto h = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        return h->_healthPercent;
    }

    void Human::SetHealth(float health) {
        auto h            = _ent.get_mut<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        h->_healthPercent = health;
        MafiaMP::Shared::RPC::HumanSetProps msg {};
        msg.health = health;
        FW_SEND_SERVER_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::HumanSetProps, _ent, msg);
    }

    void Human::Register(v8::Isolate *isolate, v8pp::module *rootModule) {
        if (!rootModule) {
            return;
        }

        v8pp::class_<Human> cls(isolate);
        cls.inherit<Framework::Integrations::Scripting::Entity>();
        cls.function("addWeapon", &Human::AddWeapon);
        cls.function("destroy", &Human::Destroy);

        cls.function("getHealth", &Human::GetHealth);
        cls.function("getVehicle", &Human::GetVehicle);
        cls.function("getVehicleSeat", &Human::GetVehicleSeat);

        cls.function("setHealth", &Human::SetHealth);

        rootModule->class_("Human", cls);
    }

    v8::Local<v8::Object> Human::WrapHuman(v8::Isolate *isolate, flecs::entity e) {
        return v8pp::class_<Scripting::Human>::create_object(isolate, e.id());
    }
} // namespace MafiaMP::Scripting
