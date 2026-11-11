#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include "core/server.h"

#include "shared/entities/human_entity.h"
#include "shared/entities/register_entities.h"
#include "shared/entities/vehicle_entity.h"

#include <core_modules.h>
#include <scripting/builtins/entity.h>
#include <scripting/builtins/entity_collection.h>
#include <networking/replication/replication_manager.h>

#include "player.h"
#include "vehicle.h"

namespace MafiaMP::Scripting {
    using PlayerCollection  = Framework::Scripting::Builtins::EntityCollection<Player, Shared::Entities::HumanEntity, Framework::Scripting::Builtins::ViewersOnly>;
    using VehicleCollection = Framework::Scripting::Builtins::EntityCollection<Vehicle, Shared::Entities::VehicleEntity>;

    class World final {
      public:
        static v8::Local<v8::Value> CreateVehicle(v8::Isolate *isolate, std::string modelName) {
            auto *engine = Framework::CoreModules::GetReplication();
            if (!engine) {
                return v8::Undefined(isolate);
            }
            auto *entity = dynamic_cast<Shared::Entities::VehicleEntity *>(engine->CreateEntity(Shared::Entities::VehicleTypeId()));
            if (!entity) {
                return v8::Undefined(isolate);
            }
            entity->modelName       = modelName;
            entity->streaming.range = Shared::Entities::VehicleEntity::kStreamRange;
            return v8pp::class_<Vehicle>::create_object(isolate, entity->GetNetworkID());
        }

        static float GetDayTimeHours() {
            auto *server = Server::_serverRef;
            return server ? server->GetEnvironment().dayTimeHours : 0.0f;
        }

        static void SetDayTimeHours(float dayTimeHours) {
            if (auto *server = Server::_serverRef) {
                server->SetDayTimeHours(dayTimeHours);
            }
        }

        static std::string GetWeatherSet() {
            auto *server = Server::_serverRef;
            return server ? server->GetEnvironment().weatherSet : "";
        }

        static void SetWeatherSet(std::string weatherSetName) {
            if (auto *server = Server::_serverRef) {
                server->SetWeatherSet(weatherSetName);
            }
        }

        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
            if (!isolate || global.IsEmpty()) {
                return;
            }

            // Ensure the entity classes exist for v8pp type conversion.
            Framework::Scripting::Builtins::Entity::GetClass(isolate);
            Vehicle::GetClass(isolate);
            Player::GetClass(isolate);

            auto ctx      = isolate->GetCurrentContext();
            auto worldObj = v8::Object::New(isolate);

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

            // setLocationVariant(location, variant): the state layer a story location streams, e.g.
            // ("hotel", "during") for its mission interior; an empty variant restores the default.
            worldObj
                ->Set(ctx, v8pp::to_v8(isolate, "setLocationVariant"),
                    v8::Function::New(ctx,
                        [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                            auto isolate = info.GetIsolate();
                            if (info.Length() < 2 || !info[0]->IsString() || !info[1]->IsString()) {
                                isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "setLocationVariant requires a location and a variant")));
                                return;
                            }
                            const auto location = v8pp::from_v8<std::string>(isolate, info[0]);
                            const auto variant  = v8pp::from_v8<std::string>(isolate, info[1]);
                            const auto valid    = [](const std::string &s) {
                                return s.size() <= 64 && s.find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789_") == std::string::npos;
                            };
                            if (location.empty() || !valid(location) || !valid(variant)) {
                                isolate->ThrowException(v8::Exception::TypeError(v8pp::to_v8(isolate, "setLocationVariant: invalid location or variant")));
                                return;
                            }
                            if (auto *server = Server::_serverRef) {
                                server->SetLocationVariant(location, variant);
                            }
                        })
                        .ToLocalChecked())
                .Check();

            // setDoorsOpen(open): unlock every door and let players use it.
            worldObj
                ->Set(ctx, v8pp::to_v8(isolate, "setDoorsOpen"),
                    v8::Function::New(ctx,
                        [](const v8::FunctionCallbackInfo<v8::Value> &info) {
                            if (auto *server = Server::_serverRef) {
                                server->SetAllDoorsOpen(info.Length() < 1 || info[0]->BooleanValue(info.GetIsolate()));
                            }
                        })
                        .ToLocalChecked())
                .Check();

            worldObj->Set(ctx, v8pp::to_v8(isolate, "players"), Framework::Scripting::Builtins::CreateCollectionObject<PlayerCollection>(isolate)).Check();
            worldObj->Set(ctx, v8pp::to_v8(isolate, "vehicles"), Framework::Scripting::Builtins::CreateCollectionObject<VehicleCollection>(isolate)).Check();

            global->Set(ctx, v8pp::to_v8(isolate, "World"), worldObj).Check();
        }
    };
} // namespace MafiaMP::Scripting
