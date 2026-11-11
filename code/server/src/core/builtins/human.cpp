#include "human.h"

#include "vehicle.h"

#include "core/server.h"
#include "shared/outfits.h"
#include "shared/rpc/ids.h"

#include <core_modules.h>
#include <networking/network_peer.h>
#include <networking/replication/replication_manager.h>

#include <mafianet/bit_stream.h>

namespace MafiaMP::Scripting {

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
    net->GetRPC()->Signal(net->ResolveIdentifier(Shared::RPC::kHumanAddWeapon), &bs, MafiaNet::Priority::High, MafiaNet::Reliability::ReliableOrdered, 0, MafiaNet::UNASSIGNED_RAKNET_GUID, true, false);
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

namespace {
    // A hash written in decimal or with a 0x prefix, else the profile path hashed like the game's
    // C_HashName (FNV-1, 64-bit). Zero means invalid.
    uint64_t ParseSpawnProfile(const std::string &value) {
        if (value.empty()) {
            return 0;
        }
        const bool hex       = value.size() > 2 && value[0] == '0' && (value[1] == 'x' || value[1] == 'X');
        const auto digits    = hex ? value.substr(2) : value;
        const auto allowed   = hex ? "0123456789abcdefABCDEF" : "0123456789";
        const bool isNumeric = !digits.empty() && digits.find_first_not_of(allowed) == std::string::npos;
        if (isNumeric) {
            try {
                return std::stoull(digits, nullptr, hex ? 16 : 10);
            }
            catch (const std::exception &) {
                return 0;
            }
        }
        uint64_t hash = 0xcbf29ce484222325ULL;
        for (const unsigned char c : value) {
            hash = (hash * 0x100000001b3ULL) ^ c;
        }
        return hash;
    }
} // namespace

bool Human::SetSkin(const std::string &skin) {
    auto *h                = ResolveHuman();
    const uint64_t profile = ParseSpawnProfile(skin);
    if (!h || profile == 0) {
        return false;
    }
    h->modelHash = profile;
    // Remote clients get it through the deltas, the owner through the forced state.
    h->ForceState();
    return true;
}

std::string Human::SetOutfit(const std::vector<Shared::Outfits::Piece> &pieces, const std::string &templateProfile) {
    // The default spawn profile is the template unless the script names one, e.g. a female body.
    constexpr uint64_t kDefaultTemplate = 335218123840277515ULL;
    auto *h                             = ResolveHuman();
    auto *server                        = Server::_serverRef;
    const uint64_t base                 = templateProfile.empty() ? kDefaultTemplate : ParseSpawnProfile(templateProfile);
    const uint64_t profile              = (h && server && base) ? server->GetOutfits().Define(pieces, base) : 0;
    if (profile == 0) {
        return "";
    }
    h->modelHash = profile;
    h->ForceState();
    return std::to_string(profile);
}

std::string Human::GetSkin() const {
    auto *h = ResolveHuman();
    return h ? std::to_string(h->modelHash) : "0";
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
    return _classes.Get(isolate, [isolate]() {
        // v8pp inherit<Player> requires Player registered first.
        Framework::Scripting::Builtins::Player::GetClass(isolate);

        auto cls = std::make_unique<v8pp::class_<Human>>(isolate);
        cls->inherit<Framework::Scripting::Builtins::Player>()
            .auto_wrap_objects(true)
            .ctor<uint64_t>()
            .function("toString", &Human::ToString)
            .function("addWeapon", &Human::AddWeapon)
            .function("setSkin", &Human::SetSkin)
            .property("skin", &Human::GetSkin)
            .property("aiming", &Human::IsAiming)
            .property("firing", &Human::IsFiring)
            .property("aimDir", &Human::GetAimDir)
            .property("aimPos", &Human::GetAimPos)
            .property("health", &Human::GetHealth, &Human::SetHealth)
            .property("weaponId", &Human::GetWeaponId)
            .property("nickname", &Human::GetNickname)
            .property("vehicleSeatIndex", &Human::GetVehicleSeatIndex);

        // vehicle returns a wrapped Vehicle handle and needs the isolate, so it stays bespoke.
        auto vehicleGetter = v8::FunctionTemplate::New(isolate, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
            auto *self = v8pp::class_<Human>::unwrap_object(info.GetIsolate(), info.This());
            if (self) {
                info.GetReturnValue().Set(self->GetVehicle(info.GetIsolate()));
            }
        });
        cls->accessor_property("vehicle", vehicleGetter, v8::Local<v8::FunctionTemplate>(), {});

        // setOutfit(pieces, template?): pieces as [piece, package] or {piece, package} ids, written
        // in decimal or with 0x. Returns the outfit's spawn profile, as skin does, or "" if invalid.
        cls->prototype_function("setOutfit", [](const v8::FunctionCallbackInfo<v8::Value> &info) {
            auto *isolate = info.GetIsolate();
            auto ctx      = isolate->GetCurrentContext();
            auto *self    = v8pp::class_<Human>::unwrap_object(isolate, info.This());
            if (!self || info.Length() < 1 || !info[0]->IsArray()) {
                isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "setOutfit requires an array of pieces")));
                return;
            }
            auto id = [&](v8::Local<v8::Value> value) {
                return value->IsString() ? ParseSpawnProfile(v8pp::from_v8<std::string>(isolate, value)) : 0;
            };
            auto list = info[0].As<v8::Array>();
            std::vector<Shared::Outfits::Piece> pieces;
            for (uint32_t i = 0; i < list->Length(); ++i) {
                auto item = list->Get(ctx, i).ToLocalChecked();
                Shared::Outfits::Piece piece;
                if (item->IsArray() && item.As<v8::Array>()->Length() >= 2) {
                    piece = {id(item.As<v8::Array>()->Get(ctx, 0).ToLocalChecked()), id(item.As<v8::Array>()->Get(ctx, 1).ToLocalChecked())};
                }
                else if (item->IsObject()) {
                    auto object = item.As<v8::Object>();
                    piece       = {id(object->Get(ctx, v8pp::to_v8(isolate, "piece")).ToLocalChecked()), id(object->Get(ctx, v8pp::to_v8(isolate, "package")).ToLocalChecked())};
                }
                if (piece.pieceId == 0 || piece.packageId == 0) {
                    isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "setOutfit: a piece needs a piece and a package id")));
                    return;
                }
                pieces.push_back(piece);
            }
            const std::string templateProfile = (info.Length() >= 2 && info[1]->IsString()) ? v8pp::from_v8<std::string>(isolate, info[1]) : "";
            info.GetReturnValue().Set(v8pp::to_v8(isolate, self->SetOutfit(pieces, templateProfile)));
        });
        return cls;
    });
}

void Human::UnregisterIsolate(v8::Isolate *isolate) {
    _classes.Unregister(isolate);
}

void Human::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
    v8pp::class_<Human> &cls = GetClass(isolate);
    auto ctx                 = isolate->GetCurrentContext();
    global->Set(ctx, v8pp::to_v8(isolate, "Human"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
}

} // namespace MafiaMP::Scripting
