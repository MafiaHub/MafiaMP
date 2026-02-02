#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>
#include <v8pp/module.hpp>

#include "core/server.h"
#include "core_modules.h"

#include "core/modules/environment.h"
#include "core/modules/vehicle.h"

#include "shared/rpc/environment.h"

#include "entity_collection.h"
#include "player.h"
#include "vehicle.h"

namespace MafiaMP::Scripting {
    class World final {
      public:
        static v8::Local<v8::Object> CreateVehicle(v8::Isolate *isolate, std::string modelName) {
            auto e = MafiaMP::Core::Modules::Vehicle::Create(Server::_serverRef);

            auto &frame     = e.ensure<Framework::World::Modules::Base::Frame>();
            frame.modelName = modelName;

            return v8pp::class_<Vehicle>::create_object(isolate, e);
        }

        static float GetDayTimeHours() {
            auto world = Framework::CoreModules::GetWorldEngine()->GetWorld();

            auto weather = world->get_mut<Core::Modules::Environment::Weather>();
            return weather->_dayTimeHours;
        }

        static void SetDayTimeHours(float dayTimeHours) {
            auto world = Framework::CoreModules::GetWorldEngine()->GetWorld();

            auto weather           = world->get_mut<Core::Modules::Environment::Weather>();
            weather->_dayTimeHours = dayTimeHours;
            FW_SEND_COMPONENT_RPC(MafiaMP::Shared::RPC::SetEnvironment, {}, weather->_dayTimeHours);
        }

        static std::string GetWeatherSet() {
            auto world = Framework::CoreModules::GetWorldEngine()->GetWorld();

            auto weather = world->get_mut<Core::Modules::Environment::Weather>();
            return weather->_weatherSetName;
        }

        static void SetWeatherSet(std::string weatherSetName) {
            auto world = Framework::CoreModules::GetWorldEngine()->GetWorld();

            auto weather             = world->get_mut<Core::Modules::Environment::Weather>();
            weather->_weatherSetName = weatherSetName;
            FW_SEND_COMPONENT_RPC(MafiaMP::Shared::RPC::SetEnvironment, SLNet::RakString(weather->_weatherSetName.c_str()), {});
        }

        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
            if (!isolate || global.IsEmpty()) {
                return;
            }

            // Ensure Entity and Vehicle classes are initialized for v8pp type conversion
            Entity::GetClass(isolate);
            Vehicle::GetClass(isolate);
            Player::GetClass(isolate);

            auto ctx = isolate->GetCurrentContext();

            // Create the World object
            auto worldObj = v8::Object::New(isolate);

            // Add methods
            worldObj->Set(ctx, v8pp::to_v8(isolate, "createVehicle"),
                          v8::Function::New(ctx, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                              auto isolate = info.GetIsolate();
                              if (info.Length() < 1 || !info[0]->IsString()) {
                                  isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "createVehicle requires a model name string")));
                                  return;
                              }
                              v8::String::Utf8Value modelName(isolate, info[0]);
                              info.GetReturnValue().Set(World::CreateVehicle(isolate, *modelName));
                          }).ToLocalChecked()).Check();

            worldObj->Set(ctx, v8pp::to_v8(isolate, "getDayTimeHours"),
                          v8::Function::New(ctx, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                              info.GetReturnValue().Set(World::GetDayTimeHours());
                          }).ToLocalChecked()).Check();

            worldObj->Set(ctx, v8pp::to_v8(isolate, "setDayTimeHours"),
                          v8::Function::New(ctx, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                              auto isolate = info.GetIsolate();
                              if (info.Length() < 1 || !info[0]->IsNumber()) {
                                  isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "setDayTimeHours requires a number")));
                                  return;
                              }
                              World::SetDayTimeHours(static_cast<float>(info[0]->NumberValue(isolate->GetCurrentContext()).FromMaybe(0.0)));
                          }).ToLocalChecked()).Check();

            worldObj->Set(ctx, v8pp::to_v8(isolate, "getWeatherSet"),
                          v8::Function::New(ctx, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                              info.GetReturnValue().Set(v8pp::to_v8(info.GetIsolate(), World::GetWeatherSet()));
                          }).ToLocalChecked()).Check();

            worldObj->Set(ctx, v8pp::to_v8(isolate, "setWeatherSet"),
                          v8::Function::New(ctx, [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                              auto isolate = info.GetIsolate();
                              if (info.Length() < 1 || !info[0]->IsString()) {
                                  isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "setWeatherSet requires a string")));
                                  return;
                              }
                              v8::String::Utf8Value weatherSet(isolate, info[0]);
                              World::SetWeatherSet(*weatherSet);
                          }).ToLocalChecked()).Check();

            // Add players collection with array-like methods
            worldObj->Set(ctx, v8pp::to_v8(isolate, "players"),
                          CreateCollectionObject<PlayerCollection>(isolate)).Check();

            // Add vehicles collection with array-like methods
            worldObj->Set(ctx, v8pp::to_v8(isolate, "vehicles"),
                          CreateCollectionObject<VehicleCollection>(isolate)).Check();

            global->Set(ctx, v8pp::to_v8(isolate, "World"), worldObj).Check();
        }
    };
} // namespace MafiaMP::Scripting
