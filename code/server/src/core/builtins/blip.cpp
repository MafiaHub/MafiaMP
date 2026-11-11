#include "blip.h"

#include "core/server.h"

#include <scripting/builtins/entity.h>
#include <scripting/engine_helpers.h>

#include <sstream>
#include <unordered_map>

namespace MafiaMP::Scripting {
    namespace {
        Core::Blips *Blips() {
            return Server::_serverRef ? &Server::_serverRef->GetBlips() : nullptr;
        }

        // The icons that show on the map, by short name. The vehicle icons show on a followed vehicle
        // while it is near; elsewhere the blip shows as a generic icon.
        const std::unordered_map<std::string, std::string> kIcons = {
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
        };
    } // namespace

    Blip::Blip(uint32_t id): _id(id) {}

    std::string Blip::ResolveIcon(const std::string &name) {
        if (const auto it = kIcons.find(name); it != kIcons.end()) {
            return it->second;
        }
        // A game icon name goes into the client's Lua as a string literal.
        const bool valid = !name.empty() && name.size() <= 64 && name.find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789_-") == std::string::npos;
        return valid ? name : std::string();
    }

    uint64_t Blip::EntityId(v8::Isolate *isolate, v8::Local<v8::Value> value) {
        auto *entity = v8pp::class_<Framework::Scripting::Builtins::Entity>::unwrap_object(isolate, value);
        return entity ? entity->GetId() : 0;
    }

    bool Blip::Exists() const {
        auto *blips = Blips();
        return blips && blips->Get(_id);
    }

    std::string Blip::ToString() const {
        std::ostringstream ss;
        ss << "Blip{ id: " << _id << " }";
        return ss.str();
    }

    Framework::Scripting::Builtins::Vector3 Blip::GetPosition() const {
        auto *blips      = Blips();
        const auto *blip = blips ? blips->Get(_id) : nullptr;
        return blip ? Framework::Scripting::Builtins::Vector3(blip->position) : Framework::Scripting::Builtins::Vector3(0, 0, 0);
    }

    void Blip::SetPosition(const Framework::Scripting::Builtins::Vector3 &position) {
        if (auto *blips = Blips()) {
            blips->SetPosition(_id, position.vec());
        }
    }

    std::string Blip::GetIcon() const {
        auto *blips      = Blips();
        const auto *blip = blips ? blips->Get(_id) : nullptr;
        return blip ? blip->icon : std::string();
    }

    void Blip::SetIcon(const std::string &icon) {
        auto *blips         = Blips();
        const auto gameIcon = ResolveIcon(icon);
        if (blips && !gameIcon.empty()) {
            blips->SetIcon(_id, gameIcon);
        }
    }

    void Blip::Destroy() {
        if (auto *blips = Blips()) {
            blips->Destroy(_id);
        }
    }

    v8pp::class_<Blip> &Blip::GetClass(v8::Isolate *isolate) {
        return _classes.Get(isolate, [isolate]() {
            auto cls = std::make_unique<v8pp::class_<Blip>>(isolate);
            cls->auto_wrap_objects(true);
            cls->ctor<uint32_t>()
                .function("toString", &Blip::ToString)
                .function("destroy", &Blip::Destroy)
                .property("id", &Blip::GetId)
                .property("exists", &Blip::Exists)
                .property("position", &Blip::GetPosition, &Blip::SetPosition)
                .property("icon", &Blip::GetIcon, &Blip::SetIcon);

            // attachTo(entity|null): follow a player, human or vehicle, or stay where it is.
            cls->prototype_function("attachTo", [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                auto *isolate = info.GetIsolate();
                auto *self    = v8pp::class_<Blip>::unwrap_object(isolate, info.This());
                auto *blips   = Blips();
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
            });

            // showRoute(player) / hideRoute(player): the player's GPS route to this blip. A player
            // has one route at a time.
            cls->prototype_function("showRoute", [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                auto *isolate           = info.GetIsolate();
                auto *self              = v8pp::class_<Blip>::unwrap_object(isolate, info.This());
                auto *blips             = Blips();
                const uint64_t playerId = info.Length() >= 1 ? EntityId(isolate, info[0]) : 0;
                if (!self || !blips) {
                    return;
                }
                if (playerId == 0) {
                    Framework::Scripting::ThrowTypeError(isolate, "showRoute: expected a player");
                    return;
                }
                blips->SetRoute(playerId, self->GetId());
            });
            cls->prototype_function("hideRoute", [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                auto *isolate           = info.GetIsolate();
                auto *self              = v8pp::class_<Blip>::unwrap_object(isolate, info.This());
                auto *blips             = Blips();
                const uint64_t playerId = info.Length() >= 1 ? EntityId(isolate, info[0]) : 0;
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
            });
            return cls;
        });
    }

    void Blip::UnregisterIsolate(v8::Isolate *isolate) {
        _classes.Unregister(isolate);
    }

    void Blip::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
        v8pp::class_<Blip> &cls = GetClass(isolate);
        auto ctx                = isolate->GetCurrentContext();
        global->Set(ctx, v8pp::to_v8(isolate, "Blip"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
    }
} // namespace MafiaMP::Scripting
