#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include <flecs.h>
#include <core_modules.h>

#include "player.h"
#include "vehicle.h"

#include "shared/modules/human_sync.hpp"
#include "shared/modules/vehicle_sync.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace MafiaMP::Scripting {

/**
 * EntityCollection provides array-like iteration methods for entity collections.
 * Supports forEach, filter, find, map operations with JavaScript callbacks.
 *
 * Usage from JS:
 *   World.players.forEach(p => p.sendChat("Hello"));
 *   World.players.filter(p => p.health > 50);
 *   World.players.find(p => p.nickname === "Admin");
 *   World.vehicles.map(v => v.position);
 */
template <typename EntityType, typename ComponentType>
class EntityCollection {
  public:
    EntityCollection() = default;

    // Get count of entities
    int GetLength(v8::Isolate *isolate) const {
        auto *world = Framework::CoreModules::GetWorldEngine()->GetWorld();
        int count   = 0;
        world->each<ComponentType>([&count](flecs::entity, ComponentType &) {
            count++;
        });
        return count;
    }

    // forEach - iterate over each entity, calling callback(entity)
    void ForEach(v8::Isolate *isolate, v8::Local<v8::Function> callback) {
        auto *world = Framework::CoreModules::GetWorldEngine()->GetWorld();
        auto ctx    = isolate->GetCurrentContext();

        world->each<ComponentType>([&](flecs::entity e, ComponentType &) {
            EntityType *entity     = new EntityType(e);
            v8::Local<v8::Object> jsEntity = EntityType::GetClass(isolate).import_external(isolate, entity);

            v8::Local<v8::Value> argv[1] = {jsEntity};
            callback->Call(ctx, v8::Undefined(isolate), 1, argv).ToLocalChecked();
        });
    }

    // filter - return array of entities matching predicate callback(entity) => bool
    v8::Local<v8::Array> Filter(v8::Isolate *isolate, v8::Local<v8::Function> callback) {
        auto *world = Framework::CoreModules::GetWorldEngine()->GetWorld();
        auto ctx    = isolate->GetCurrentContext();

        std::vector<v8::Local<v8::Value>> results;

        world->each<ComponentType>([&](flecs::entity e, ComponentType &) {
            EntityType *entity     = new EntityType(e);
            v8::Local<v8::Object> jsEntity = EntityType::GetClass(isolate).import_external(isolate, entity);

            v8::Local<v8::Value> argv[1] = {jsEntity};
            v8::Local<v8::Value> result  = callback->Call(ctx, v8::Undefined(isolate), 1, argv).ToLocalChecked();

            if (result->BooleanValue(isolate)) {
                results.push_back(jsEntity);
            }
        });

        v8::Local<v8::Array> arr = v8::Array::New(isolate, static_cast<int>(results.size()));
        for (size_t i = 0; i < results.size(); i++) {
            arr->Set(ctx, static_cast<uint32_t>(i), results[i]).Check();
        }
        return arr;
    }

    // find - return first entity matching predicate callback(entity) => bool, or undefined
    v8::Local<v8::Value> Find(v8::Isolate *isolate, v8::Local<v8::Function> callback) {
        auto *world = Framework::CoreModules::GetWorldEngine()->GetWorld();
        auto ctx    = isolate->GetCurrentContext();

        v8::Local<v8::Value> found = v8::Undefined(isolate);
        bool foundOne              = false;

        world->each<ComponentType>([&](flecs::entity e, ComponentType &) {
            if (foundOne) return;

            EntityType *entity     = new EntityType(e);
            v8::Local<v8::Object> jsEntity = EntityType::GetClass(isolate).import_external(isolate, entity);

            v8::Local<v8::Value> argv[1] = {jsEntity};
            v8::Local<v8::Value> result  = callback->Call(ctx, v8::Undefined(isolate), 1, argv).ToLocalChecked();

            if (result->BooleanValue(isolate)) {
                found    = jsEntity;
                foundOne = true;
            }
        });

        return found;
    }

    // map - return array of callback results for each entity
    v8::Local<v8::Array> Map(v8::Isolate *isolate, v8::Local<v8::Function> callback) {
        auto *world = Framework::CoreModules::GetWorldEngine()->GetWorld();
        auto ctx    = isolate->GetCurrentContext();

        std::vector<v8::Local<v8::Value>> results;

        world->each<ComponentType>([&](flecs::entity e, ComponentType &) {
            EntityType *entity     = new EntityType(e);
            v8::Local<v8::Object> jsEntity = EntityType::GetClass(isolate).import_external(isolate, entity);

            v8::Local<v8::Value> argv[1] = {jsEntity};
            v8::Local<v8::Value> result  = callback->Call(ctx, v8::Undefined(isolate), 1, argv).ToLocalChecked();

            results.push_back(result);
        });

        v8::Local<v8::Array> arr = v8::Array::New(isolate, static_cast<int>(results.size()));
        for (size_t i = 0; i < results.size(); i++) {
            arr->Set(ctx, static_cast<uint32_t>(i), results[i]).Check();
        }
        return arr;
    }

    // some - return true if any entity matches predicate
    bool Some(v8::Isolate *isolate, v8::Local<v8::Function> callback) {
        auto *world = Framework::CoreModules::GetWorldEngine()->GetWorld();
        auto ctx    = isolate->GetCurrentContext();
        bool found  = false;

        world->each<ComponentType>([&](flecs::entity e, ComponentType &) {
            if (found) return;

            EntityType *entity     = new EntityType(e);
            v8::Local<v8::Object> jsEntity = EntityType::GetClass(isolate).import_external(isolate, entity);

            v8::Local<v8::Value> argv[1] = {jsEntity};
            v8::Local<v8::Value> result  = callback->Call(ctx, v8::Undefined(isolate), 1, argv).ToLocalChecked();

            if (result->BooleanValue(isolate)) {
                found = true;
            }
        });

        return found;
    }

    // every - return true if all entities match predicate
    bool Every(v8::Isolate *isolate, v8::Local<v8::Function> callback) {
        auto *world = Framework::CoreModules::GetWorldEngine()->GetWorld();
        auto ctx    = isolate->GetCurrentContext();
        bool allMatch = true;

        world->each<ComponentType>([&](flecs::entity e, ComponentType &) {
            if (!allMatch) return;

            EntityType *entity     = new EntityType(e);
            v8::Local<v8::Object> jsEntity = EntityType::GetClass(isolate).import_external(isolate, entity);

            v8::Local<v8::Value> argv[1] = {jsEntity};
            v8::Local<v8::Value> result  = callback->Call(ctx, v8::Undefined(isolate), 1, argv).ToLocalChecked();

            if (!result->BooleanValue(isolate)) {
                allMatch = false;
            }
        });

        return allMatch;
    }
};

// Type aliases for convenience
using PlayerCollection  = EntityCollection<Player, Shared::Modules::HumanSync::UpdateData>;
using VehicleCollection = EntityCollection<Vehicle, Shared::Modules::VehicleSync::UpdateData>;

/**
 * Helper to register a collection object on the World module.
 * Creates a JS object with forEach, filter, find, map, some, every methods and length property.
 */
template <typename CollectionType>
v8::Local<v8::Object> CreateCollectionObject(v8::Isolate *isolate) {
    auto ctx = isolate->GetCurrentContext();
    auto obj = v8::Object::New(isolate);

    // forEach method
    obj->Set(ctx, v8pp::to_v8(isolate, "forEach"),
             v8::Function::New(ctx, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                 auto isolate = info.GetIsolate();
                 if (info.Length() < 1 || !info[0]->IsFunction()) {
                     isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "forEach requires a callback function")));
                     return;
                 }
                 CollectionType collection;
                 collection.ForEach(isolate, info[0].As<v8::Function>());
             }).ToLocalChecked()).Check();

    // filter method
    obj->Set(ctx, v8pp::to_v8(isolate, "filter"),
             v8::Function::New(ctx, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                 auto isolate = info.GetIsolate();
                 if (info.Length() < 1 || !info[0]->IsFunction()) {
                     isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "filter requires a callback function")));
                     return;
                 }
                 CollectionType collection;
                 info.GetReturnValue().Set(collection.Filter(isolate, info[0].As<v8::Function>()));
             }).ToLocalChecked()).Check();

    // find method
    obj->Set(ctx, v8pp::to_v8(isolate, "find"),
             v8::Function::New(ctx, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                 auto isolate = info.GetIsolate();
                 if (info.Length() < 1 || !info[0]->IsFunction()) {
                     isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "find requires a callback function")));
                     return;
                 }
                 CollectionType collection;
                 info.GetReturnValue().Set(collection.Find(isolate, info[0].As<v8::Function>()));
             }).ToLocalChecked()).Check();

    // map method
    obj->Set(ctx, v8pp::to_v8(isolate, "map"),
             v8::Function::New(ctx, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                 auto isolate = info.GetIsolate();
                 if (info.Length() < 1 || !info[0]->IsFunction()) {
                     isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "map requires a callback function")));
                     return;
                 }
                 CollectionType collection;
                 info.GetReturnValue().Set(collection.Map(isolate, info[0].As<v8::Function>()));
             }).ToLocalChecked()).Check();

    // some method
    obj->Set(ctx, v8pp::to_v8(isolate, "some"),
             v8::Function::New(ctx, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                 auto isolate = info.GetIsolate();
                 if (info.Length() < 1 || !info[0]->IsFunction()) {
                     isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "some requires a callback function")));
                     return;
                 }
                 CollectionType collection;
                 info.GetReturnValue().Set(collection.Some(isolate, info[0].As<v8::Function>()));
             }).ToLocalChecked()).Check();

    // every method
    obj->Set(ctx, v8pp::to_v8(isolate, "every"),
             v8::Function::New(ctx, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                 auto isolate = info.GetIsolate();
                 if (info.Length() < 1 || !info[0]->IsFunction()) {
                     isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "every requires a callback function")));
                     return;
                 }
                 CollectionType collection;
                 info.GetReturnValue().Set(collection.Every(isolate, info[0].As<v8::Function>()));
             }).ToLocalChecked()).Check();

    // length property (getter)
    obj->SetAccessor(ctx, v8pp::to_v8(isolate, "length").As<v8::Name>(),
                     [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                         CollectionType collection;
                         info.GetReturnValue().Set(collection.GetLength(info.GetIsolate()));
                     }).Check();

    return obj;
}

} // namespace MafiaMP::Scripting
