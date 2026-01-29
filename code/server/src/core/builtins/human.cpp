#include "human.h"

#include "core/server.h"

#include "shared/game_rpc/human/human_add_weapon.h"
#include "shared/game_rpc/human/human_setprops.h"

#include "shared/modules/human_sync.hpp"

#include "vehicle.h"

namespace MafiaMP::Scripting {

std::unique_ptr<v8pp::class_<Human>> Human::_class;

std::string Human::ToString() const {
    std::ostringstream ss;
    ss << "Human{ id: " << _ent.id() << " }";
    return ss.str();
}

bool Human::IsAiming() const {
    const auto h = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    return h->weaponData.isAiming;
}

bool Human::IsFiring() const {
    const auto h = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    return h->weaponData.isFiring;
}

void Human::AddWeapon(int weaponId, int ammo) {
    FW_SEND_SERVER_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::HumanAddWeapon, _ent, weaponId, ammo);
}

Framework::Scripting::Builtins::Vector3 Human::GetAimDir() const {
    const auto h   = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    const auto dir = h->weaponData.aimDir;
    return Framework::Scripting::Builtins::Vector3(dir.x, dir.y, dir.z);
}

Framework::Scripting::Builtins::Vector3 Human::GetAimPos() const {
    const auto h   = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    const auto pos = h->weaponData.aimPos;
    return Framework::Scripting::Builtins::Vector3(pos.x, pos.y, pos.z);
}

float Human::GetHealth() const {
    const auto h = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    return h->_healthPercent;
}

void Human::SetHealth(float health) {
    auto h            = _ent.get_mut<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    h->_healthPercent = health;
    MafiaMP::Shared::RPC::HumanSetProps msg {};
    msg.health = health;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::HumanSetProps, _ent, msg);
}

uint16_t Human::GetWeaponId() const {
    const auto h = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    return h->weaponData.currentWeaponId;
}

uint64_t Human::GetVehicleId() const {
    const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    const auto carEnt     = flecs::entity(_ent.world(), updateData->carPassenger.carId);
    if (carEnt.is_valid() && carEnt.is_alive()) {
        return carEnt.id();
    }
    return 0;
}

int Human::GetVehicleSeatIndex() const {
    const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    const auto carEnt     = flecs::entity(_ent.world(), updateData->carPassenger.carId);
    if (carEnt.is_valid() && carEnt.is_alive()) {
        return updateData->carPassenger.seatId;
    }
    return -1;
}

v8pp::class_<Human> &Human::GetClass(v8::Isolate *isolate) {
    if (!_class) {
        _class = std::make_unique<v8pp::class_<Human>>(isolate);
        _class->inherit<Entity>()
            .ctor<flecs::entity_t>()
            .set("toString", &Human::ToString)
            .set("isAiming", &Human::IsAiming)
            .set("isFiring", &Human::IsFiring)
            .set("addWeapon", &Human::AddWeapon)
            .set("getAimDir", &Human::GetAimDir)
            .set("getAimPos", &Human::GetAimPos)
            .set("getHealth", &Human::GetHealth)
            .set("setHealth", &Human::SetHealth)
            .set("getVehicleId", &Human::GetVehicleId)
            .set("getVehicleSeatIndex", &Human::GetVehicleSeatIndex)
            .set("getWeaponId", &Human::GetWeaponId);
    }
    return *_class;
}

void Human::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
    v8pp::class_<Human> &cls = GetClass(isolate);
    auto ctx                 = isolate->GetCurrentContext();
    global->Set(ctx, v8pp::to_v8(isolate, "Human"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
}

} // namespace MafiaMP::Scripting
