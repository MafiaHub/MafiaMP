#include "vehicle.h"

#include "core/server.h"

#include "shared/constants.h"
#include "shared/game_rpc/vehicle/vehicle_setprops.h"
#include "shared/modules/vehicle_sync.hpp"

#include "player.h"

#include <integrations/server/scripting/module.h>
#include <logging/logger.h>
#include <scripting/node_engine.h>

namespace MafiaMP::Scripting {

std::unique_ptr<v8pp::class_<Vehicle>> Vehicle::_class;

namespace {
    // Helper to emit vehicle events with common V8 setup boilerplate
    template <typename ArgsBuilder>
    void EmitVehicleEvent(const std::string &eventName, ArgsBuilder &&buildArgs) {
        auto server = MafiaMP::Server::_serverRef;
        if (!server)
            return;

        auto scriptingModule = server->GetScriptingModule();
        if (!scriptingModule)
            return;

        auto *engine          = scriptingModule->GetEngine();
        auto *resourceManager = scriptingModule->GetResourceManager();
        if (!engine || !resourceManager || !engine->IsInitialized())
            return;

        v8::Isolate *isolate = engine->GetIsolate();
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);
        v8::Local<v8::Context> context = engine->GetContext();
        v8::Context::Scope contextScope(context);

        std::vector<v8::Local<v8::Value>> args = buildArgs(isolate);
        resourceManager->GetEvents().EmitReserved(isolate, context, eventName, args);
    }
} // namespace

void Vehicle::EventVehiclePlayerEnter(flecs::entity vehicle, flecs::entity player, int seatIndex) {
    Framework::Logging::GetLogger("Scripting")->debug("Vehicle {} player {} enter seat {}", vehicle.id(), player.id(), seatIndex);

    EmitVehicleEvent("vehiclePlayerEnter", [&](v8::Isolate *isolate) {
        std::vector<v8::Local<v8::Value>> args;
        args.push_back(v8pp::class_<Vehicle>::create_object(isolate, vehicle));
        args.push_back(v8pp::class_<Player>::create_object(isolate, player));
        args.push_back(v8::Integer::New(isolate, seatIndex));
        return args;
    });
}

void Vehicle::EventVehiclePlayerLeave(flecs::entity vehicle, flecs::entity player) {
    Framework::Logging::GetLogger("Scripting")->debug("Vehicle {} player {} leave", vehicle.id(), player.id());

    EmitVehicleEvent("vehiclePlayerLeave", [&](v8::Isolate *isolate) {
        std::vector<v8::Local<v8::Value>> args;
        args.push_back(v8pp::class_<Vehicle>::create_object(isolate, vehicle));
        args.push_back(v8pp::class_<Player>::create_object(isolate, player));
        return args;
    });
}

std::string Vehicle::ToString() const {
    std::ostringstream ss;
    ss << "Vehicle{ id: " << _ent.id() << " }";
    return ss.str();
}

bool Vehicle::GetBeaconLightsOn() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return vehData->beaconLightsOn;
}

void Vehicle::SetBeaconLightsOn(bool on) {
    auto vehData            = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->beaconLightsOn = on;
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.beaconLightsOn = on;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

Framework::Scripting::Builtins::Color Vehicle::GetColorPrimary() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return Framework::Scripting::Builtins::Color(vehData->colorPrimary);
}

void Vehicle::SetColorPrimary(Framework::Scripting::Builtins::Color color) {
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

    vehData->colorPrimary = color.vec();
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.colorPrimary = color.vec();
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

Framework::Scripting::Builtins::Color Vehicle::GetColorSecondary() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return Framework::Scripting::Builtins::Color(vehData->colorSecondary);
}

void Vehicle::SetColorSecondary(Framework::Scripting::Builtins::Color color) {
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

    vehData->colorSecondary = color.vec();
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.colorSecondary = color.vec();
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

float Vehicle::GetDirt() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return vehData->dirt;
}

void Vehicle::SetDirt(float dirt) {
    auto vehData  = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->dirt = dirt;
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.dirt = dirt;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

bool Vehicle::GetEngineOn() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return vehData->engineOn;
}

void Vehicle::SetEngineOn(bool on) {
    auto vehData      = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->engineOn = on;
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.engineOn = on;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

float Vehicle::GetFuel() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return vehData->fuel;
}

void Vehicle::SetFuel(float fuel) {
    auto vehData  = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->fuel = fuel;
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.fuel = fuel;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

std::string Vehicle::GetLicensePlate() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return vehData->licensePlate;
}

void Vehicle::SetLicensePlate(std::string plate) {
    auto vehData      = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    size_t copyLength = std::min<size_t>(Shared::Constants::VEHICLE_LICENSE_PLATE_MAX_LENGTH - 1, plate.length());
    std::memcpy(vehData->licensePlate, plate.c_str(), copyLength);
    vehData->licensePlate[copyLength] = '\0';

    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.licensePlate = plate.c_str();
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

int Vehicle::GetLockState() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return static_cast<int>(vehData->lockState);
}

void Vehicle::SetLockState(int lockState) {
    if (lockState < 0 || lockState > 2) {
        Framework::Logging::GetLogger("Scripting")->warn("Invalid lockState value: {}", lockState);
        return;
    }
    auto vehData       = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->lockState = static_cast<Shared::Modules::VehicleSync::LockState>(lockState);
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.lockState = vehData->lockState;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

bool Vehicle::GetRadioOn() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return vehData->radioOn;
}

void Vehicle::SetRadioOn(bool on) {
    auto vehData     = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->radioOn = on;
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.radioOn = on;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

int Vehicle::GetRadioStationId() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return vehData->radioStationId;
}

void Vehicle::SetRadioStationId(int id) {
    auto vehData            = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->radioStationId = id;
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.radioStationId = id;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

Framework::Scripting::Builtins::Color Vehicle::GetRimColor() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return Framework::Scripting::Builtins::Color(vehData->rimColor);
}

void Vehicle::SetRimColor(Framework::Scripting::Builtins::Color color) {
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

    vehData->rimColor = color.vec();
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.rimColor = color.vec();
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

float Vehicle::GetRust() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return vehData->rust;
}

void Vehicle::SetRust(float rust) {
    auto vehData  = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->rust = rust;
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.rust = rust;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

bool Vehicle::GetSirenOn() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return vehData->sirenOn;
}

void Vehicle::SetSirenOn(bool on) {
    auto vehData     = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->sirenOn = on;
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.sirenOn = on;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

Framework::Scripting::Builtins::Color Vehicle::GetTireColor() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return Framework::Scripting::Builtins::Color(vehData->tireColor);
}

void Vehicle::SetTireColor(Framework::Scripting::Builtins::Color color) {
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

    vehData->tireColor = color.vec();
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.tireColor = color.vec();
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

Framework::Scripting::Builtins::Color Vehicle::GetWindowTint() {
    auto vehData = _ent.get<Shared::Modules::VehicleSync::UpdateData>();
    return Framework::Scripting::Builtins::Color(vehData->windowTint);
}

void Vehicle::SetWindowTint(Framework::Scripting::Builtins::Color color) {
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

    vehData->windowTint = color.vec();
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.windowTint = color.vec();
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

v8pp::class_<Vehicle> &Vehicle::GetClass(v8::Isolate *isolate) {
    if (!_class) {
        _class = std::make_unique<v8pp::class_<Vehicle>>(isolate);
        _class->auto_wrap_objects(true);
        _class->inherit<Entity>()
            .ctor<flecs::entity_t>()
            .function("toString", &Vehicle::ToString);

        auto protoTemplate = _class->class_function_template()->PrototypeTemplate();

        // beaconLightsOn
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "beaconLightsOn").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetBeaconLightsOn());
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsBoolean()) self->SetBeaconLightsOn(value->BooleanValue(info.GetIsolate()));
            });

        // colorPrimary
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "colorPrimary").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) {
                    auto color = self->GetColorPrimary();
                    auto &cls  = Framework::Scripting::Builtins::Color::GetClass(info.GetIsolate());
                    info.GetReturnValue().Set(cls.import_external(info.GetIsolate(), new Framework::Scripting::Builtins::Color(color)));
                }
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self  = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                auto *color = v8pp::class_<Framework::Scripting::Builtins::Color>::unwrap_object(info.GetIsolate(), value);
                if (self && color) self->SetColorPrimary(*color);
            });

        // colorSecondary
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "colorSecondary").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) {
                    auto color = self->GetColorSecondary();
                    auto &cls  = Framework::Scripting::Builtins::Color::GetClass(info.GetIsolate());
                    info.GetReturnValue().Set(cls.import_external(info.GetIsolate(), new Framework::Scripting::Builtins::Color(color)));
                }
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self  = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                auto *color = v8pp::class_<Framework::Scripting::Builtins::Color>::unwrap_object(info.GetIsolate(), value);
                if (self && color) self->SetColorSecondary(*color);
            });

        // dirt
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "dirt").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetDirt());
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsNumber()) {
                    self->SetDirt(static_cast<float>(value->NumberValue(info.GetIsolate()->GetCurrentContext()).FromMaybe(0.0)));
                }
            });

        // engineOn
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "engineOn").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetEngineOn());
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsBoolean()) self->SetEngineOn(value->BooleanValue(info.GetIsolate()));
            });

        // fuel
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "fuel").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetFuel());
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsNumber()) {
                    self->SetFuel(static_cast<float>(value->NumberValue(info.GetIsolate()->GetCurrentContext()).FromMaybe(0.0)));
                }
            });

        // licensePlate
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "licensePlate").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(v8pp::to_v8(info.GetIsolate(), self->GetLicensePlate()));
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsString()) {
                    v8::String::Utf8Value str(info.GetIsolate(), value);
                    self->SetLicensePlate(*str);
                }
            });

        // lockState
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "lockState").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetLockState());
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsInt32()) {
                    self->SetLockState(value->Int32Value(info.GetIsolate()->GetCurrentContext()).FromMaybe(0));
                }
            });

        // radioOn
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "radioOn").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetRadioOn());
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsBoolean()) self->SetRadioOn(value->BooleanValue(info.GetIsolate()));
            });

        // radioStationId
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "radioStationId").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetRadioStationId());
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsInt32()) {
                    self->SetRadioStationId(value->Int32Value(info.GetIsolate()->GetCurrentContext()).FromMaybe(0));
                }
            });

        // rimColor
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "rimColor").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) {
                    auto color = self->GetRimColor();
                    auto &cls  = Framework::Scripting::Builtins::Color::GetClass(info.GetIsolate());
                    info.GetReturnValue().Set(cls.import_external(info.GetIsolate(), new Framework::Scripting::Builtins::Color(color)));
                }
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self  = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                auto *color = v8pp::class_<Framework::Scripting::Builtins::Color>::unwrap_object(info.GetIsolate(), value);
                if (self && color) self->SetRimColor(*color);
            });

        // rust
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "rust").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetRust());
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsNumber()) {
                    self->SetRust(static_cast<float>(value->NumberValue(info.GetIsolate()->GetCurrentContext()).FromMaybe(0.0)));
                }
            });

        // sirenOn
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "sirenOn").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set(self->GetSirenOn());
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsBoolean()) self->SetSirenOn(value->BooleanValue(info.GetIsolate()));
            });

        // tireColor
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "tireColor").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) {
                    auto color = self->GetTireColor();
                    auto &cls  = Framework::Scripting::Builtins::Color::GetClass(info.GetIsolate());
                    info.GetReturnValue().Set(cls.import_external(info.GetIsolate(), new Framework::Scripting::Builtins::Color(color)));
                }
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self  = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                auto *color = v8pp::class_<Framework::Scripting::Builtins::Color>::unwrap_object(info.GetIsolate(), value);
                if (self && color) self->SetTireColor(*color);
            });

        // windowTint
        protoTemplate->SetNativeDataProperty(
            v8pp::to_v8(isolate, "windowTint").As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) {
                    auto color = self->GetWindowTint();
                    auto &cls  = Framework::Scripting::Builtins::Color::GetClass(info.GetIsolate());
                    info.GetReturnValue().Set(cls.import_external(info.GetIsolate(), new Framework::Scripting::Builtins::Color(color)));
                }
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self  = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                auto *color = v8pp::class_<Framework::Scripting::Builtins::Color>::unwrap_object(info.GetIsolate(), value);
                if (self && color) self->SetWindowTint(*color);
            });
    }
    return *_class;
}

void Vehicle::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
    v8pp::class_<Vehicle> &cls = GetClass(isolate);
    auto ctx                   = isolate->GetCurrentContext();
    global->Set(ctx, v8pp::to_v8(isolate, "Vehicle"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
}

} // namespace MafiaMP::Scripting
