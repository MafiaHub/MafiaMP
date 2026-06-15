#include "human.h"

#include "vehicle.h"

#include "shared/rpc/ids.h"

#include <core_modules.h>
#include <networking/network_peer.h>
#include <scripting/builtins/property.h>
#include <networking/replication/replication_manager.h>

#include <mafianet/BitStream.h>

namespace MafiaMP::Scripting {

std::unique_ptr<v8pp::class_<Human>> Human::_class;

Human::Human(uint64_t networkId): Framework::Scripting::Builtins::Player(networkId) {
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
    net->GetRPC()->Signal(Shared::RPC::kHumanAddWeapon, &bs, MafiaNet::Priority::High, MafiaNet::Reliability::ReliableOrdered, 0, MafiaNet::UNASSIGNED_RAKNET_GUID, true, false);
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
    auto *repl = Framework::CoreModules::GetReplication();
    return (repl && repl->GetEntityByNetworkID(carId)) ? carId : 0;
}

int Human::GetVehicleSeatIndex() const {
    auto *h = ResolveHuman();
    if (!h || GetVehicleId() == 0) return -1;
    return h->data.carPassenger.seatId;
}

v8pp::class_<Human> &Human::GetClass(v8::Isolate *isolate) {
    if (!_class) {
        // v8pp inherit<Player> requires Player registered first.
        Framework::Scripting::Builtins::Player::GetClass(isolate);

        _class = std::make_unique<v8pp::class_<Human>>(isolate);
        _class->inherit<Framework::Scripting::Builtins::Player>()
            .auto_wrap_objects(true)
            .ctor<uint64_t>()
            .function("toString", &Human::ToString)
            .function("addWeapon", &Human::AddWeapon);

        auto protoTemplate = _class->class_function_template()->PrototypeTemplate();

        using namespace Framework::Scripting::Builtins;

        RegisterReadonlyProperty<Human, &Human::IsAiming>(isolate, protoTemplate, "aiming");
        RegisterReadonlyProperty<Human, &Human::IsFiring>(isolate, protoTemplate, "firing");
        RegisterReadonlyObjectProperty<Human, &Human::GetAimDir>(isolate, protoTemplate, "aimDir");
        RegisterReadonlyObjectProperty<Human, &Human::GetAimPos>(isolate, protoTemplate, "aimPos");
        RegisterProperty<Human, &Human::GetHealth, &Human::SetHealth>(isolate, protoTemplate, "health");
        RegisterReadonlyProperty<Human, &Human::GetWeaponId>(isolate, protoTemplate, "weaponId");
        RegisterReadonlyProperty<Human, &Human::GetNickname>(isolate, protoTemplate, "nickname");
        RegisterReadonlyProperty<Human, &Human::GetVehicleSeatIndex>(isolate, protoTemplate, "vehicleSeatIndex");

        // vehicle returns a wrapped Vehicle handle and needs the isolate, so it stays bespoke.
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "vehicle").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetVehicle(info.GetIsolate()));
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
