#include "blip.h"

#include "player.h"

#include "core/server.h"

#include <scripting/builtins/entity.h>
#include <scripting/engine_helpers.h>

#include <array>
#include <sstream>
#include <string_view>
#include <utility>

namespace MafiaMP::Scripting {
    namespace {
        Core::Blips *GetBlips() {
            return Server::_serverRef ? &Server::_serverRef->GetBlips() : nullptr;
        }

        // The icons that show on the map, by short name. The vehicle icons show on a followed vehicle
        // while it is near; elsewhere the blip shows as a generic icon.
        constexpr std::array<std::pair<std::string_view, std::string_view>, 11> kIcons = {{
            {"car", "player_car"},
            {"police_car", "police_car"},
            {"taxi", "taxi_car"},
            {"enemy_car", "enemy"},
            {"generic", "freeride-generic"},
            {"challenge", "freeride-challenge"},
            {"challenge_complete", "freeride-challenge-complete"},
            {"weather", "change-weather"},
            {"warning", "objective_warning"},
            {"crime", "witnessed_crime"},
            {"traps", "traps"},
        }};

        // The NetworkID of an Entity handle (player, human or vehicle); 0 for anything else.
        uint64_t EntityId(v8::Isolate *isolate, v8::Local<v8::Value> value) {
            auto *entity = v8pp::class_<Framework::Scripting::Builtins::Entity>::unwrap_object(isolate, value);
            return entity ? entity->GetId() : 0;
        }

        // The human NetworkID of a Player handle; 0 for anything else.
        uint64_t PlayerId(v8::Isolate *isolate, v8::Local<v8::Value> value) {
            auto *player = v8pp::class_<Player>::unwrap_object(isolate, value);
            return player ? player->GetId() : 0;
        }
    } // namespace

    Blip::Blip(uint32_t id): _id(id) {}

    std::string Blip::ResolveIcon(const std::string &name) {
        for (const auto &[shortName, gameName] : kIcons) {
            if (name == shortName) {
                return std::string(gameName);
            }
        }
        const bool valid = !name.empty() && name.size() <= 64 && name.find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789_-") == std::string::npos;
        return valid ? name : std::string();
    }

    bool Blip::Exists() const {
        auto *blips = GetBlips();
        return blips && blips->Get(_id);
    }

    std::string Blip::ToString() const {
        std::ostringstream ss;
        ss << "Blip{ id: " << _id << " }";
        return ss.str();
    }

    Framework::Scripting::Builtins::Vector3 Blip::GetPosition() const {
        auto *blips      = GetBlips();
        const auto *blip = blips ? blips->Get(_id) : nullptr;
        return blip ? Framework::Scripting::Builtins::Vector3(blip->position) : Framework::Scripting::Builtins::Vector3(0, 0, 0);
    }

    void Blip::SetPosition(const Framework::Scripting::Builtins::Vector3 &position) {
        if (auto *blips = GetBlips()) {
            blips->SetPosition(_id, position.vec());
        }
    }

    std::string Blip::GetIcon() const {
        auto *blips      = GetBlips();
        const auto *blip = blips ? blips->Get(_id) : nullptr;
        if (!blip) {
            return std::string();
        }
        for (const auto &[shortName, gameName] : kIcons) {
            if (blip->icon == gameName) {
                return std::string(shortName);
            }
        }
        return blip->icon;
    }

    void Blip::SetIcon(const std::string &icon) {
        auto *blips         = GetBlips();
        const auto gameIcon = ResolveIcon(icon);
        if (gameIcon.empty()) {
            Framework::Scripting::ThrowTypeError(v8::Isolate::GetCurrent(), "Blip.icon: unknown icon");
            return;
        }
        if (blips) {
            blips->SetIcon(_id, gameIcon);
        }
    }

    void Blip::Destroy() {
        if (auto *blips = GetBlips()) {
            blips->Destroy(_id);
        }
        if (const auto it = _created.find(v8::Isolate::GetCurrent()); it != _created.end()) {
            it->second.erase(_id);
        }
    }

    void Blip::Create(const v8::FunctionCallbackInfo<v8::Value> &info) {
        auto *isolate  = info.GetIsolate();
        auto *position = info.Length() >= 1 ? v8pp::class_<Framework::Scripting::Builtins::Vector3>::unwrap_object(isolate, info[0]) : nullptr;
        auto *blips    = GetBlips();
        if (!position || !blips) {
            Framework::Scripting::ThrowTypeError(isolate, "Blip.create: expected a Vector3 position");
            return;
        }
        std::string icon = "generic";
        if (info.Length() >= 2 && !info[1]->IsNullOrUndefined()) {
            icon = v8pp::from_v8<std::string>(isolate, info[1], "");
        }
        const auto gameIcon = ResolveIcon(icon);
        if (gameIcon.empty()) {
            Framework::Scripting::ThrowTypeError(isolate, "Blip.create: unknown icon");
            return;
        }
        uint64_t visibleTo = 0;
        if (info.Length() >= 3 && !info[2]->IsNullOrUndefined()) {
            visibleTo = PlayerId(isolate, info[2]);
            if (visibleTo == 0) {
                Framework::Scripting::ThrowTypeError(isolate, "Blip.create: expected a player (or null for everyone)");
                return;
            }
        }
        const uint32_t id = blips->Create(position->vec(), gameIcon, visibleTo);
        _created[isolate].insert(id);
        GetClass(isolate);
        info.GetReturnValue().Set(v8pp::class_<Blip>::create_object(isolate, id));
    }

    // attachTo(entity|null): follow a player, human or vehicle, or stay where it is.
    void Blip::AttachTo(const v8::FunctionCallbackInfo<v8::Value> &info) {
        auto *isolate = info.GetIsolate();
        auto *self    = v8pp::class_<Blip>::unwrap_object(isolate, info.This());
        auto *blips   = GetBlips();
        if (!self || !blips) {
            return;
        }
        uint64_t entityId = 0;
        if (info.Length() >= 1 && !info[0]->IsNullOrUndefined()) {
            entityId = EntityId(isolate, info[0]);
            if (entityId == 0) {
                Framework::Scripting::ThrowTypeError(isolate, "attachTo: expected an entity (or null to detach)");
                return;
            }
        }
        blips->AttachTo(self->GetId(), entityId);
    }

    // showRoute(player): the player's GPS route to this blip. A player has one route at a time.
    void Blip::ShowRoute(const v8::FunctionCallbackInfo<v8::Value> &info) {
        auto *isolate           = info.GetIsolate();
        auto *self              = v8pp::class_<Blip>::unwrap_object(isolate, info.This());
        auto *blips             = GetBlips();
        const uint64_t playerId = info.Length() >= 1 ? PlayerId(isolate, info[0]) : 0;
        if (!self || !blips) {
            return;
        }
        if (playerId == 0) {
            Framework::Scripting::ThrowTypeError(isolate, "showRoute: expected a player");
            return;
        }
        blips->SetRoute(playerId, self->GetId());
    }

    // hideRoute(player): clears the player's route if it leads to this blip.
    void Blip::HideRoute(const v8::FunctionCallbackInfo<v8::Value> &info) {
        auto *isolate           = info.GetIsolate();
        auto *self              = v8pp::class_<Blip>::unwrap_object(isolate, info.This());
        auto *blips             = GetBlips();
        const uint64_t playerId = info.Length() >= 1 ? PlayerId(isolate, info[0]) : 0;
        if (!self || !blips) {
            return;
        }
        if (playerId == 0) {
            Framework::Scripting::ThrowTypeError(isolate, "hideRoute: expected a player");
            return;
        }
        if (blips->GetRoute(playerId) == self->GetId()) {
            blips->SetRoute(playerId, 0);
        }
    }

    v8pp::class_<Blip> &Blip::GetClass(v8::Isolate *isolate) {
        return _classes.Get(isolate, [isolate]() {
            auto cls = std::make_unique<v8pp::class_<Blip>>(isolate);
            cls->auto_wrap_objects(true);
            cls->ctor<uint32_t>()
                .static_function("create", &Blip::Create)
                .function("toString", &Blip::ToString)
                .function("destroy", &Blip::Destroy)
                .property("id", &Blip::GetId)
                .property("exists", &Blip::Exists)
                .property("position", &Blip::GetPosition, &Blip::SetPosition)
                .property("icon", &Blip::GetIcon, &Blip::SetIcon);
            cls->prototype_function("attachTo", &Blip::AttachTo);
            cls->prototype_function("showRoute", &Blip::ShowRoute);
            cls->prototype_function("hideRoute", &Blip::HideRoute);
            return cls;
        });
    }

    void Blip::UnregisterIsolate(v8::Isolate *isolate) {
        if (const auto it = _created.find(isolate); it != _created.end()) {
            if (auto *blips = GetBlips()) {
                for (const uint32_t id : it->second) {
                    blips->Destroy(id);
                }
            }
            _created.erase(it);
        }
        _classes.Unregister(isolate);
    }

    void Blip::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
        v8pp::class_<Blip> &cls = GetClass(isolate);
        auto ctx                = isolate->GetCurrentContext();
        global->Set(ctx, v8pp::to_v8(isolate, "Blip"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
    }
} // namespace MafiaMP::Scripting
