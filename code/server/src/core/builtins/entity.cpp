#include "entity.h"

#include <core_modules.h>
#include <world/engine.h>

#include <fmt/format.h>
#include <glm/gtc/quaternion.hpp>

namespace MafiaMP::Scripting {

std::unique_ptr<v8pp::class_<Entity>> Entity::_class;

Entity::Entity(flecs::entity_t ent) {
    _ent = flecs::entity(Framework::CoreModules::GetWorldEngine()->GetWorld()->get_world(), ent);
    if (!_ent.is_valid() || !_ent.is_alive()) {
        throw std::runtime_error(fmt::format("Entity handle '{}' is not valid!", ent));
    }
}

Framework::Scripting::Builtins::Vector3 Entity::GetPosition() const {
    const auto tr = _ent.get<Framework::World::Modules::Base::Transform>();
    if (tr) {
        return Framework::Scripting::Builtins::Vector3(tr->pos.x, tr->pos.y, tr->pos.z);
    }
    return Framework::Scripting::Builtins::Vector3(0, 0, 0);
}

void Entity::SetPosition(const Framework::Scripting::Builtins::Vector3 &pos) {
    auto tr = _ent.get_mut<Framework::World::Modules::Base::Transform>();
    if (tr) {
        tr->pos = pos.vec();
        tr->IncrementGeneration();
    }
}

Framework::Scripting::Builtins::Vector3 Entity::GetRotation() const {
    const auto tr = _ent.get<Framework::World::Modules::Base::Transform>();
    if (tr) {
        glm::vec3 euler = glm::eulerAngles(tr->rot);
        return Framework::Scripting::Builtins::Vector3(glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z));
    }
    return Framework::Scripting::Builtins::Vector3(0, 0, 0);
}

void Entity::SetRotationFromEuler(const Framework::Scripting::Builtins::Vector3 &rot) {
    auto tr = _ent.get_mut<Framework::World::Modules::Base::Transform>();
    if (tr) {
        glm::vec3 radians(glm::radians(rot.vec().x), glm::radians(rot.vec().y), glm::radians(rot.vec().z));
        tr->rot = glm::quat(radians);
        tr->IncrementGeneration();
    }
}

void Entity::SetRotationFromQuaternion(const Framework::Scripting::Builtins::Quaternion &quat) {
    auto tr = _ent.get_mut<Framework::World::Modules::Base::Transform>();
    if (tr) {
        tr->rot = quat.quat();
        tr->IncrementGeneration();
    }
}

std::string Entity::GetModelName() const {
    const auto frame = _ent.get<Framework::World::Modules::Base::Frame>();
    if (frame) {
        return frame->modelName;
    }
    return "";
}

std::string Entity::ToString() const {
    std::ostringstream ss;
    ss << "Entity{ id: " << _ent.id() << " }";
    return ss.str();
}

v8pp::class_<Entity> &Entity::GetClass(v8::Isolate *isolate) {
    if (!_class) {
        _class = std::make_unique<v8pp::class_<Entity>>(isolate);
        _class->auto_wrap_objects(true);
        _class->ctor<flecs::entity_t>()
            .set("toString", &Entity::ToString);

        // Add properties using SetAccessor for JS-native property access
        auto protoTemplate = _class->class_function_template()->PrototypeTemplate();

        // Read-only property: id
        protoTemplate->SetAccessor(
            v8pp::to_v8(isolate, "id").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Entity>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(static_cast<double>(self->GetId()));
            });

        // Read-only property: modelName
        protoTemplate->SetAccessor(
            v8pp::to_v8(isolate, "modelName").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Entity>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(v8pp::to_v8(info.GetIsolate(), self->GetModelName()));
            });

        // Property: position (Vector3)
        protoTemplate->SetAccessor(
            v8pp::to_v8(isolate, "position").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Entity>::unwrap_object(info.GetIsolate(), info.This());
                if (self) {
                    auto pos = self->GetPosition();
                    auto &cls = Framework::Scripting::Builtins::Vector3::GetClass(info.GetIsolate());
                    info.GetReturnValue().Set(cls.import_external(info.GetIsolate(), new Framework::Scripting::Builtins::Vector3(pos)));
                }
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Entity>::unwrap_object(info.GetIsolate(), info.This());
                if (!self) return;

                auto *vec = v8pp::class_<Framework::Scripting::Builtins::Vector3>::unwrap_object(info.GetIsolate(), value);
                if (vec) {
                    self->SetPosition(*vec);
                }
            });

        // Property: rotation (accepts both Vector3 euler degrees and Quaternion)
        protoTemplate->SetAccessor(
            v8pp::to_v8(isolate, "rotation").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Entity>::unwrap_object(info.GetIsolate(), info.This());
                if (self) {
                    auto rot = self->GetRotation();
                    auto &cls = Framework::Scripting::Builtins::Vector3::GetClass(info.GetIsolate());
                    info.GetReturnValue().Set(cls.import_external(info.GetIsolate(), new Framework::Scripting::Builtins::Vector3(rot)));
                }
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Entity>::unwrap_object(info.GetIsolate(), info.This());
                if (!self) return;

                // Try to unwrap as Vector3 first (euler angles in degrees)
                auto *vec = v8pp::class_<Framework::Scripting::Builtins::Vector3>::unwrap_object(info.GetIsolate(), value);
                if (vec) {
                    self->SetRotationFromEuler(*vec);
                    return;
                }

                // Try to unwrap as Quaternion
                auto *quat = v8pp::class_<Framework::Scripting::Builtins::Quaternion>::unwrap_object(info.GetIsolate(), value);
                if (quat) {
                    self->SetRotationFromQuaternion(*quat);
                    return;
                }

                // Neither type matched - throw an error
                info.GetIsolate()->ThrowException(v8::Exception::TypeError(
                    v8pp::to_v8(info.GetIsolate(), "rotation must be a Vector3 (euler degrees) or Quaternion")));
            });
    }
    return *_class;
}

void Entity::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
    v8pp::class_<Entity> &cls = GetClass(isolate);
    auto ctx                  = isolate->GetCurrentContext();
    global->Set(ctx, v8pp::to_v8(isolate, "Entity"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
}

} // namespace MafiaMP::Scripting
