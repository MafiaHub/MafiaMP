#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include <core_modules.h>
#include <networking/replication/replication_manager.h>

#include "shared/entities/human_entity.h"
#include "shared/entities/vehicle_entity.h"

#include "player.h"
#include "vehicle.h"

#include <type_traits>
#include <vector>

namespace MafiaMP::Scripting {

// Array-like JS iteration over a category of replicated entities.
//
// Usage from JS:
//   World.players.forEach(p => p.sendChat("Hello"));
//   World.vehicles.filter(v => v.engineOn);
//
// JsType is the scripting wrapper (Player/Vehicle); NativeType is the replicated entity
// (HumanEntity/VehicleEntity). For humans only viewer entities (actual players) are included.
template <typename JsType, typename NativeType>
class EntityCollection {
  public:
    EntityCollection() = default;

    template <typename Fn>
    static void ForEachNative(Fn &&fn) {
        auto *repl = Framework::CoreModules::GetReplication();
        if (!repl) {
            return;
        }
        repl->ForEachEntity([&](Framework::Networking::Replication::NetworkEntity *e) {
            auto *typed = dynamic_cast<NativeType *>(e);
            if (!typed) {
                return;
            }
            if constexpr (std::is_same_v<NativeType, Shared::Entities::HumanEntity>) {
                if (!typed->isViewer) {
                    return; // players only
                }
            }
            fn(typed);
        });
    }

    int GetLength(v8::Isolate *) const {
        int count = 0;
        ForEachNative([&count](NativeType *) {
            count++;
        });
        return count;
    }

    void ForEach(v8::Isolate *isolate, v8::Local<v8::Function> callback) {
        auto ctx          = isolate->GetCurrentContext();
        bool hadException = false;
        ForEachNative([&](NativeType *e) {
            if (hadException) return;
            v8::Local<v8::Object> jsEntity = JsType::GetClass(isolate).import_external(isolate, new JsType(e->GetNetworkID()));
            v8::Local<v8::Value> argv[1]   = {jsEntity};
            v8::TryCatch try_catch(isolate);
            v8::Local<v8::Value> result;
            if (!callback->Call(ctx, v8::Undefined(isolate), 1, argv).ToLocal(&result)) {
                if (try_catch.HasCaught()) isolate->ThrowException(try_catch.Exception());
                hadException = true;
            }
        });
    }

    v8::Local<v8::Array> Filter(v8::Isolate *isolate, v8::Local<v8::Function> callback) {
        auto ctx          = isolate->GetCurrentContext();
        bool hadException = false;
        std::vector<v8::Local<v8::Value>> results;
        ForEachNative([&](NativeType *e) {
            if (hadException) return;
            v8::Local<v8::Object> jsEntity = JsType::GetClass(isolate).import_external(isolate, new JsType(e->GetNetworkID()));
            v8::Local<v8::Value> argv[1]   = {jsEntity};
            v8::TryCatch try_catch(isolate);
            v8::Local<v8::Value> result;
            if (!callback->Call(ctx, v8::Undefined(isolate), 1, argv).ToLocal(&result)) {
                if (try_catch.HasCaught()) isolate->ThrowException(try_catch.Exception());
                hadException = true;
                return;
            }
            if (result->BooleanValue(isolate)) results.push_back(jsEntity);
        });
        if (hadException) return v8::Array::New(isolate, 0);
        v8::Local<v8::Array> arr = v8::Array::New(isolate, static_cast<int>(results.size()));
        for (size_t i = 0; i < results.size(); i++) arr->Set(ctx, static_cast<uint32_t>(i), results[i]).Check();
        return arr;
    }

    v8::Local<v8::Value> Find(v8::Isolate *isolate, v8::Local<v8::Function> callback) {
        auto ctx                   = isolate->GetCurrentContext();
        v8::Local<v8::Value> found = v8::Undefined(isolate);
        bool foundOne = false, hadException = false;
        ForEachNative([&](NativeType *e) {
            if (foundOne || hadException) return;
            v8::Local<v8::Object> jsEntity = JsType::GetClass(isolate).import_external(isolate, new JsType(e->GetNetworkID()));
            v8::Local<v8::Value> argv[1]   = {jsEntity};
            v8::TryCatch try_catch(isolate);
            v8::Local<v8::Value> result;
            if (!callback->Call(ctx, v8::Undefined(isolate), 1, argv).ToLocal(&result)) {
                if (try_catch.HasCaught()) isolate->ThrowException(try_catch.Exception());
                hadException = true;
                return;
            }
            if (result->BooleanValue(isolate)) {
                found    = jsEntity;
                foundOne = true;
            }
        });
        return hadException ? v8::Undefined(isolate) : found;
    }

    v8::Local<v8::Array> Map(v8::Isolate *isolate, v8::Local<v8::Function> callback) {
        auto ctx          = isolate->GetCurrentContext();
        bool hadException = false;
        std::vector<v8::Local<v8::Value>> results;
        ForEachNative([&](NativeType *e) {
            if (hadException) return;
            v8::Local<v8::Object> jsEntity = JsType::GetClass(isolate).import_external(isolate, new JsType(e->GetNetworkID()));
            v8::Local<v8::Value> argv[1]   = {jsEntity};
            v8::TryCatch try_catch(isolate);
            v8::Local<v8::Value> result;
            if (!callback->Call(ctx, v8::Undefined(isolate), 1, argv).ToLocal(&result)) {
                if (try_catch.HasCaught()) isolate->ThrowException(try_catch.Exception());
                hadException = true;
                return;
            }
            results.push_back(result);
        });
        if (hadException) return v8::Array::New(isolate, 0);
        v8::Local<v8::Array> arr = v8::Array::New(isolate, static_cast<int>(results.size()));
        for (size_t i = 0; i < results.size(); i++) arr->Set(ctx, static_cast<uint32_t>(i), results[i]).Check();
        return arr;
    }

    bool Some(v8::Isolate *isolate, v8::Local<v8::Function> callback) {
        auto ctx = isolate->GetCurrentContext();
        bool found = false, hadException = false;
        ForEachNative([&](NativeType *e) {
            if (found || hadException) return;
            v8::Local<v8::Object> jsEntity = JsType::GetClass(isolate).import_external(isolate, new JsType(e->GetNetworkID()));
            v8::Local<v8::Value> argv[1]   = {jsEntity};
            v8::TryCatch try_catch(isolate);
            v8::Local<v8::Value> result;
            if (!callback->Call(ctx, v8::Undefined(isolate), 1, argv).ToLocal(&result)) {
                if (try_catch.HasCaught()) isolate->ThrowException(try_catch.Exception());
                hadException = true;
                return;
            }
            if (result->BooleanValue(isolate)) found = true;
        });
        return hadException ? false : found;
    }

    bool Every(v8::Isolate *isolate, v8::Local<v8::Function> callback) {
        auto ctx = isolate->GetCurrentContext();
        bool allMatch = true, hadException = false;
        ForEachNative([&](NativeType *e) {
            if (!allMatch || hadException) return;
            v8::Local<v8::Object> jsEntity = JsType::GetClass(isolate).import_external(isolate, new JsType(e->GetNetworkID()));
            v8::Local<v8::Value> argv[1]   = {jsEntity};
            v8::TryCatch try_catch(isolate);
            v8::Local<v8::Value> result;
            if (!callback->Call(ctx, v8::Undefined(isolate), 1, argv).ToLocal(&result)) {
                if (try_catch.HasCaught()) isolate->ThrowException(try_catch.Exception());
                hadException = true;
                return;
            }
            if (!result->BooleanValue(isolate)) allMatch = false;
        });
        return hadException ? false : allMatch;
    }
};

using PlayerCollection  = EntityCollection<Player, Shared::Entities::HumanEntity>;
using VehicleCollection = EntityCollection<Vehicle, Shared::Entities::VehicleEntity>;

template <typename CollectionType>
v8::Local<v8::Object> CreateCollectionObject(v8::Isolate *isolate) {
    auto ctx  = isolate->GetCurrentContext();
    auto tmpl = v8::ObjectTemplate::New(isolate);

    tmpl->SetNativeDataProperty(v8pp::to_v8(isolate, "length").As<v8::Name>(),
        [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
            CollectionType collection;
            info.GetReturnValue().Set(collection.GetLength(info.GetIsolate()));
        });

    auto obj = tmpl->NewInstance(ctx).ToLocalChecked();

    auto method = [&](const char *name, void (*fn)(const v8::FunctionCallbackInfo<v8::Value> &)) {
        obj->Set(ctx, v8pp::to_v8(isolate, name), v8::Function::New(ctx, fn).ToLocalChecked()).Check();
    };

    method("forEach", [](const v8::FunctionCallbackInfo<v8::Value> &info) {
        auto isolate = info.GetIsolate();
        if (info.Length() < 1 || !info[0]->IsFunction()) {
            isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "forEach requires a callback function")));
            return;
        }
        CollectionType collection;
        collection.ForEach(isolate, info[0].As<v8::Function>());
    });
    method("filter", [](const v8::FunctionCallbackInfo<v8::Value> &info) {
        auto isolate = info.GetIsolate();
        if (info.Length() < 1 || !info[0]->IsFunction()) {
            isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "filter requires a callback function")));
            return;
        }
        CollectionType collection;
        info.GetReturnValue().Set(collection.Filter(isolate, info[0].As<v8::Function>()));
    });
    method("find", [](const v8::FunctionCallbackInfo<v8::Value> &info) {
        auto isolate = info.GetIsolate();
        if (info.Length() < 1 || !info[0]->IsFunction()) {
            isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "find requires a callback function")));
            return;
        }
        CollectionType collection;
        info.GetReturnValue().Set(collection.Find(isolate, info[0].As<v8::Function>()));
    });
    method("map", [](const v8::FunctionCallbackInfo<v8::Value> &info) {
        auto isolate = info.GetIsolate();
        if (info.Length() < 1 || !info[0]->IsFunction()) {
            isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "map requires a callback function")));
            return;
        }
        CollectionType collection;
        info.GetReturnValue().Set(collection.Map(isolate, info[0].As<v8::Function>()));
    });
    method("some", [](const v8::FunctionCallbackInfo<v8::Value> &info) {
        auto isolate = info.GetIsolate();
        if (info.Length() < 1 || !info[0]->IsFunction()) {
            isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "some requires a callback function")));
            return;
        }
        CollectionType collection;
        info.GetReturnValue().Set(collection.Some(isolate, info[0].As<v8::Function>()));
    });
    method("every", [](const v8::FunctionCallbackInfo<v8::Value> &info) {
        auto isolate = info.GetIsolate();
        if (info.Length() < 1 || !info[0]->IsFunction()) {
            isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "every requires a callback function")));
            return;
        }
        CollectionType collection;
        info.GetReturnValue().Set(collection.Every(isolate, info[0].As<v8::Function>()));
    });

    return obj;
}

} // namespace MafiaMP::Scripting
