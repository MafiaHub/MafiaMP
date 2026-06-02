#include "entity.h"

#include <core_modules.h>
#include <world/engine.h>

#include <fmt/format.h>
#include <glm/gtc/quaternion.hpp>

namespace MafiaMP::Scripting {

std::unordered_map<v8::Isolate *, std::unique_ptr<v8pp::class_<Entity>>> Entity::_classes;

Entity::Entity(uint64_t networkId): _id(networkId) {
    if (!Resolve()) {
        throw std::runtime_error(fmt::format("Entity handle '{}' is not valid!", networkId));
    }
}

Replication::NetworkEntity *Entity::Resolve() const {
    auto *world = Framework::CoreModules::GetWorldEngine();
    return world ? world->GetEntityByNetworkID(_id) : nullptr;
}

Framework::Scripting::Builtins::Vector3 Entity::GetPosition() const {
    if (auto *e = Resolve()) {
        return Framework::Scripting::Builtins::Vector3(e->position.x, e->position.y, e->position.z);
    }
    return Framework::Scripting::Builtins::Vector3(0, 0, 0);
}

void Entity::SetPosition(const Framework::Scripting::Builtins::Vector3 &pos) {
    if (auto *e = Resolve()) {
        e->position = pos.vec();
        e->ForceTransform();
    }
}

Framework::Scripting::Builtins::Vector3 Entity::GetRotation() const {
    if (auto *e = Resolve()) {
        glm::vec3 euler = glm::eulerAngles(e->rotation);
        return Framework::Scripting::Builtins::Vector3(glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z));
    }
    return Framework::Scripting::Builtins::Vector3(0, 0, 0);
}

void Entity::SetRotationFromEuler(const Framework::Scripting::Builtins::Vector3 &rot) {
    if (auto *e = Resolve()) {
        glm::vec3 radians(glm::radians(rot.vec().x), glm::radians(rot.vec().y), glm::radians(rot.vec().z));
        e->rotation = glm::quat(radians);
        e->ForceTransform();
    }
}

void Entity::SetRotationFromQuaternion(const Framework::Scripting::Builtins::Quaternion &quat) {
    if (auto *e = Resolve()) {
        e->rotation = quat.quat();
        e->ForceTransform();
    }
}

std::string Entity::GetModelName() const {
    if (auto *e = Resolve()) {
        return e->modelName;
    }
    return "";
}

std::string Entity::ToString() const {
    std::ostringstream ss;
    ss << "Entity{ id: " << _id << " }";
    return ss.str();
}

v8pp::class_<Entity> &Entity::GetClass(v8::Isolate *isolate) {
    auto it = _classes.find(isolate);
    if (it != _classes.end()) {
        return *it->second;
    }

    auto &cls = _classes[isolate];
    cls = std::make_unique<v8pp::class_<Entity>>(isolate);
    cls->auto_wrap_objects(true);
    cls->ctor<uint64_t>()
        .function("toString", &Entity::ToString);

    auto protoTemplate = cls->class_function_template()->PrototypeTemplate();

    // Read-only property: id
    protoTemplate->SetNativeDataProperty(
        v8pp::to_v8(isolate, "id").As<v8::Name>(),
        [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
            auto *self = v8pp::class_<Entity>::unwrap_object(info.GetIsolate(), info.This());
            if (self) info.GetReturnValue().Set(static_cast<double>(self->GetId()));
        });

    // Read-only property: modelName
    protoTemplate->SetNativeDataProperty(
        v8pp::to_v8(isolate, "modelName").As<v8::Name>(),
        [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
            auto *self = v8pp::class_<Entity>::unwrap_object(info.GetIsolate(), info.This());
            if (self) info.GetReturnValue().Set(v8pp::to_v8(info.GetIsolate(), self->GetModelName()));
        });

    // Property: position (Vector3)
    {
        auto positionGetter = v8::FunctionTemplate::New(isolate, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
            auto *self = v8pp::class_<Entity>::unwrap_object(info.GetIsolate(), info.This());
            if (self) {
                auto pos = self->GetPosition();
                auto &vecCls = Framework::Scripting::Builtins::Vector3::GetClass(info.GetIsolate());
                info.GetReturnValue().Set(vecCls.import_external(info.GetIsolate(), new Framework::Scripting::Builtins::Vector3(pos)));
            }
        });
        auto positionSetter = v8::FunctionTemplate::New(isolate, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
            auto *self = v8pp::class_<Entity>::unwrap_object(info.GetIsolate(), info.This());
            if (!self || info.Length() < 1) return;

            auto *vec = v8pp::class_<Framework::Scripting::Builtins::Vector3>::unwrap_object(info.GetIsolate(), info[0]);
            if (vec) {
                self->SetPosition(*vec);
            }
        });
        protoTemplate->SetAccessorProperty(v8pp::to_v8(isolate, "position").As<v8::Name>(), positionGetter, positionSetter);
    }

    // Property: rotation (accepts both Vector3 euler degrees and Quaternion)
    {
        auto rotationGetter = v8::FunctionTemplate::New(isolate, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
            auto *self = v8pp::class_<Entity>::unwrap_object(info.GetIsolate(), info.This());
            if (self) {
                auto rot = self->GetRotation();
                auto &vecCls = Framework::Scripting::Builtins::Vector3::GetClass(info.GetIsolate());
                info.GetReturnValue().Set(vecCls.import_external(info.GetIsolate(), new Framework::Scripting::Builtins::Vector3(rot)));
            }
        });
        auto rotationSetter = v8::FunctionTemplate::New(isolate, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
            auto *self = v8pp::class_<Entity>::unwrap_object(info.GetIsolate(), info.This());
            if (!self || info.Length() < 1) return;

            auto *vec = v8pp::class_<Framework::Scripting::Builtins::Vector3>::unwrap_object(info.GetIsolate(), info[0]);
            if (vec) {
                self->SetRotationFromEuler(*vec);
                return;
            }

            auto *quat = v8pp::class_<Framework::Scripting::Builtins::Quaternion>::unwrap_object(info.GetIsolate(), info[0]);
            if (quat) {
                self->SetRotationFromQuaternion(*quat);
                return;
            }

            info.GetIsolate()->ThrowException(v8::Exception::TypeError(
                v8pp::to_v8(info.GetIsolate(), "rotation must be a Vector3 (euler degrees) or Quaternion")));
        });
        protoTemplate->SetAccessorProperty(v8pp::to_v8(isolate, "rotation").As<v8::Name>(), rotationGetter, rotationSetter);
    }

    return *cls;
}

void Entity::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
    v8pp::class_<Entity> &cls = GetClass(isolate);
    auto ctx                  = isolate->GetCurrentContext();
    global->Set(ctx, v8pp::to_v8(isolate, "Entity"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
}

} // namespace MafiaMP::Scripting
