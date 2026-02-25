#include "human.h"

#include "core/server.h"

#include "shared/game_rpc/human/human_add_weapon.h"
#include "shared/game_rpc/human/human_setprops.h"

#include "shared/modules/human_sync.hpp"

#include <world/modules/base.hpp>

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
    if (!h) return false;
    return h->weaponData.isAiming;
}

bool Human::IsFiring() const {
    const auto h = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    if (!h) return false;
    return h->weaponData.isFiring;
}

void Human::AddWeapon(int weaponId, int ammo) {
    FW_SEND_SERVER_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::HumanAddWeapon, _ent, weaponId, ammo);
}

Framework::Scripting::Builtins::Vector3 Human::GetAimDir() const {
    const auto h = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    if (!h) return Framework::Scripting::Builtins::Vector3(0, 0, 0);
    const auto dir = h->weaponData.aimDir;
    return Framework::Scripting::Builtins::Vector3(dir.x, dir.y, dir.z);
}

Framework::Scripting::Builtins::Vector3 Human::GetAimPos() const {
    const auto h = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    if (!h) return Framework::Scripting::Builtins::Vector3(0, 0, 0);
    const auto pos = h->weaponData.aimPos;
    return Framework::Scripting::Builtins::Vector3(pos.x, pos.y, pos.z);
}

float Human::GetHealth() const {
    const auto h = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    if (!h) return 0.0f;
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
    if (!h) return 0;
    return h->weaponData.currentWeaponId;
}

std::string Human::GetNickname() const {
    const auto streamer = _ent.get<Framework::World::Modules::Base::Streamer>();
    if (streamer) {
        return streamer->nickname;
    }
    return "";
}

v8::Local<v8::Value> Human::GetVehicle(v8::Isolate *isolate) const {
    const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    if (!updateData) return v8::Undefined(isolate);
    const auto carEnt = flecs::entity(_ent.world(), updateData->carPassenger.carId);
    if (carEnt.is_valid() && carEnt.is_alive()) {
        Vehicle *vehicle = new Vehicle(carEnt);
        return Vehicle::GetClass(isolate).import_external(isolate, vehicle);
    }
    return v8::Undefined(isolate);
}

uint64_t Human::GetVehicleId() const {
    const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    if (!updateData) return 0;
    const auto carEnt = flecs::entity(_ent.world(), updateData->carPassenger.carId);
    if (carEnt.is_valid() && carEnt.is_alive()) {
        return carEnt.id();
    }
    return 0;
}

int Human::GetVehicleSeatIndex() const {
    const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
    if (!updateData) return -1;
    const auto carEnt = flecs::entity(_ent.world(), updateData->carPassenger.carId);
    if (carEnt.is_valid() && carEnt.is_alive()) {
        return updateData->carPassenger.seatId;
    }
    return -1;
}

v8pp::class_<Human> &Human::GetClass(v8::Isolate *isolate) {
    if (!_class) {
        _class = std::make_unique<v8pp::class_<Human>>(isolate);
        _class->inherit<Entity>()
            .auto_wrap_objects(true)
            .ctor<flecs::entity_t>()
            .function("toString", &Human::ToString)
            .function("addWeapon", &Human::AddWeapon);

        auto protoTemplate = _class->class_function_template()->PrototypeTemplate();

        // aiming (read-only)
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "aiming").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->IsAiming());
            });

        // firing (read-only)
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "firing").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->IsFiring());
            });

        // aimDir (read-only)
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "aimDir").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) {
                    auto dir = self->GetAimDir();
                    auto &cls = Framework::Scripting::Builtins::Vector3::GetClass(info.GetIsolate());
                    info.GetReturnValue().Set(cls.import_external(info.GetIsolate(), new Framework::Scripting::Builtins::Vector3(dir)));
                }
            });

        // aimPos (read-only)
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "aimPos").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) {
                    auto pos = self->GetAimPos();
                    auto &cls = Framework::Scripting::Builtins::Vector3::GetClass(info.GetIsolate());
                    info.GetReturnValue().Set(cls.import_external(info.GetIsolate(), new Framework::Scripting::Builtins::Vector3(pos)));
                }
            });

        // health
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "health").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetHealth());
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsNumber()) {
                    self->SetHealth(static_cast<float>(value->NumberValue(info.GetIsolate()->GetCurrentContext()).FromMaybe(0.0)));
                }
            });

        // weaponId (read-only)
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "weaponId").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetWeaponId());
            });

        // nickname (read-only)
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "nickname").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(v8pp::to_v8(info.GetIsolate(), self->GetNickname()));
            });

        // vehicle (read-only)
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "vehicle").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetVehicle(info.GetIsolate()));
            });

        // vehicleSeatIndex (read-only)
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "vehicleSeatIndex").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetVehicleSeatIndex());
            });
    }
    return *_class;
}

void Human::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
    v8pp::class_<Human> &cls = GetClass(isolate);
    auto ctx                 = isolate->GetCurrentContext();
    global->Set(ctx, v8pp::to_v8(isolate, "Human"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
}

} // namespace MafiaMP::Scripting
