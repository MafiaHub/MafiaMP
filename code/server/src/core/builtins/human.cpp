#include "human.h"

#include "vehicle.h"

#include "shared/rpc/ids.h"

#include <core_modules.h>
#include <networking/network_peer.h>
#include <world/engine.h>

#include <mafianet/BitStream.h>

namespace MafiaMP::Scripting {

std::unique_ptr<v8pp::class_<Human>> Human::_class;

Human::Human(uint64_t networkId): Entity(networkId) {
    if (!ResolveHuman()) {
        throw std::runtime_error("Entity handle is not a Human!");
    }
}

Shared::Entities::HumanEntity *Human::ResolveHuman() const {
    return dynamic_cast<Shared::Entities::HumanEntity *>(Resolve());
}

std::string Human::ToString() const {
    std::ostringstream ss;
    ss << "Human{ id: " << _id << " }";
    return ss.str();
}

bool Human::IsAiming() const {
    auto *h = ResolveHuman();
    return h && h->data.weaponData.isAiming;
}

bool Human::IsFiring() const {
    auto *h = ResolveHuman();
    return h && h->data.weaponData.isFiring;
}

void Human::AddWeapon(int weaponId, int ammo) {
    auto *net = Framework::CoreModules::GetNetworkPeer();
    if (!net) {
        return;
    }
    // Wire: <human NetworkID><weaponId><ammo>
    MafiaNet::BitStream bs;
    uint64_t id = _id;
    bs.Write(id);
    bs.Write(weaponId);
    bs.Write(ammo);
    net->GetRPC()->Signal(Shared::RPC::kHumanAddWeapon, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, MafiaNet::UNASSIGNED_RAKNET_GUID, true, false);
}

Framework::Scripting::Builtins::Vector3 Human::GetAimDir() const {
    auto *h = ResolveHuman();
    if (!h) return Framework::Scripting::Builtins::Vector3(0, 0, 0);
    const auto dir = h->data.weaponData.aimDir;
    return Framework::Scripting::Builtins::Vector3(dir.x, dir.y, dir.z);
}

Framework::Scripting::Builtins::Vector3 Human::GetAimPos() const {
    auto *h = ResolveHuman();
    if (!h) return Framework::Scripting::Builtins::Vector3(0, 0, 0);
    const auto pos = h->data.weaponData.aimPos;
    return Framework::Scripting::Builtins::Vector3(pos.x, pos.y, pos.z);
}

float Human::GetHealth() const {
    auto *h = ResolveHuman();
    return h ? h->data._healthPercent : 0.0f;
}

void Human::SetHealth(float health) {
    // Health is replicated state on the entity; setting it syncs via the DeltaSerializer.
    if (auto *h = ResolveHuman()) {
        h->data._healthPercent = health;
    }
}

uint16_t Human::GetWeaponId() const {
    auto *h = ResolveHuman();
    return h ? h->data.weaponData.currentWeaponId : 0;
}

std::string Human::GetNickname() const {
    auto *h = ResolveHuman();
    return h ? h->nickname : "";
}

v8::Local<v8::Value> Human::GetVehicle(v8::Isolate *isolate) const {
    const uint64_t carId = GetVehicleId();
    if (carId == 0) {
        return v8::Undefined(isolate);
    }
    Vehicle *vehicle = new Vehicle(carId);
    return Vehicle::GetClass(isolate).import_external(isolate, vehicle);
}

uint64_t Human::GetVehicleId() const {
    auto *h = ResolveHuman();
    if (!h) return 0;
    const uint64_t carId = h->data.carPassenger.carId;
    if (carId == 0) return 0;
    auto *world = Framework::CoreModules::GetWorldEngine();
    return (world && world->GetEntityByNetworkID(carId)) ? carId : 0;
}

int Human::GetVehicleSeatIndex() const {
    auto *h = ResolveHuman();
    if (!h || GetVehicleId() == 0) return -1;
    return h->data.carPassenger.seatId;
}

v8pp::class_<Human> &Human::GetClass(v8::Isolate *isolate) {
    if (!_class) {
        _class = std::make_unique<v8pp::class_<Human>>(isolate);
        _class->inherit<Entity>()
            .auto_wrap_objects(true)
            .ctor<uint64_t>()
            .function("toString", &Human::ToString)
            .function("addWeapon", &Human::AddWeapon);

        auto protoTemplate = _class->class_function_template()->PrototypeTemplate();

        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "aiming").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->IsAiming());
            });

        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "firing").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->IsFiring());
            });

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

        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "weaponId").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetWeaponId());
            });

        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "nickname").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(v8pp::to_v8(info.GetIsolate(), self->GetNickname()));
            });

        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "vehicle").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetVehicle(info.GetIsolate()));
            });

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
