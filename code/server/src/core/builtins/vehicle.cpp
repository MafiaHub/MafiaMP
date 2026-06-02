#include "vehicle.h"

#include "core/server.h"
#include "player.h"

#include "shared/constants.h"

#include <integrations/server/scripting/module.h>
#include <logging/logger.h>
#include <scripting/node_engine.h>

#include <algorithm>
#include <cstring>

namespace MafiaMP::Scripting {

std::unique_ptr<v8pp::class_<Vehicle>> Vehicle::_class;

namespace {
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

    // V8 accessor callbacks must be non-capturing function pointers. Passing the getter/setter as
    // template parameters keeps these lambdas capture-free while still being reusable per property.
    template <bool (Vehicle::*Getter)(), void (Vehicle::*Setter)(bool)>
    void RegisterBoolProp(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> proto, const char *name) {
        proto->SetNativeDataProperty(
            v8pp::to_v8(isolate, name).As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set((self->*Getter)());
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsBoolean()) (self->*Setter)(value->BooleanValue(info.GetIsolate()));
            });
    }

    template <float (Vehicle::*Getter)(), void (Vehicle::*Setter)(float)>
    void RegisterFloatProp(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> proto, const char *name) {
        proto->SetNativeDataProperty(
            v8pp::to_v8(isolate, name).As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set((self->*Getter)());
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsNumber()) (self->*Setter)(static_cast<float>(value->NumberValue(info.GetIsolate()->GetCurrentContext()).FromMaybe(0.0)));
            });
    }

    template <int (Vehicle::*Getter)(), void (Vehicle::*Setter)(int)>
    void RegisterIntProp(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> proto, const char *name) {
        proto->SetNativeDataProperty(
            v8pp::to_v8(isolate, name).As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) info.GetReturnValue().Set((self->*Getter)());
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self && value->IsInt32()) (self->*Setter)(value->Int32Value(info.GetIsolate()->GetCurrentContext()).FromMaybe(0));
            });
    }

    template <Framework::Scripting::Builtins::Color (Vehicle::*Getter)(), void (Vehicle::*Setter)(Framework::Scripting::Builtins::Color)>
    void RegisterColorProp(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> proto, const char *name) {
        proto->SetNativeDataProperty(
            v8pp::to_v8(isolate, name).As<v8::Name>(),
            [](v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value> &info) {
                auto *self = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                if (self) {
                    auto color = (self->*Getter)();
                    auto &cls  = Framework::Scripting::Builtins::Color::GetClass(info.GetIsolate());
                    info.GetReturnValue().Set(cls.import_external(info.GetIsolate(), new Framework::Scripting::Builtins::Color(color)));
                }
            },
            [](v8::Local<v8::Name>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
                auto *self  = v8pp::class_<Vehicle>::unwrap_object(info.GetIsolate(), info.This());
                auto *color = v8pp::class_<Framework::Scripting::Builtins::Color>::unwrap_object(info.GetIsolate(), value);
                if (self && color) (self->*Setter)(*color);
            });
    }
} // namespace

Vehicle::Vehicle(uint64_t networkId): Entity(networkId) {
    if (!ResolveVehicle()) {
        throw std::runtime_error("Entity handle is not a Vehicle!");
    }
}

Shared::Entities::VehicleEntity *Vehicle::ResolveVehicle() const {
    return dynamic_cast<Shared::Entities::VehicleEntity *>(Resolve());
}

void Vehicle::EventVehiclePlayerEnter(uint64_t vehicleId, uint64_t playerId, int seatIndex) {
    Framework::Logging::GetLogger("Scripting")->debug("Vehicle {} player {} enter seat {}", vehicleId, playerId, seatIndex);
    EmitVehicleEvent("vehiclePlayerEnter", [&](v8::Isolate *isolate) {
        std::vector<v8::Local<v8::Value>> args;
        args.push_back(v8pp::class_<Vehicle>::create_object(isolate, vehicleId));
        args.push_back(v8pp::class_<Player>::create_object(isolate, playerId));
        args.push_back(v8::Integer::New(isolate, seatIndex));
        return args;
    });
}

void Vehicle::EventVehiclePlayerLeave(uint64_t vehicleId, uint64_t playerId) {
    Framework::Logging::GetLogger("Scripting")->debug("Vehicle {} player {} leave", vehicleId, playerId);
    EmitVehicleEvent("vehiclePlayerLeave", [&](v8::Isolate *isolate) {
        std::vector<v8::Local<v8::Value>> args;
        args.push_back(v8pp::class_<Vehicle>::create_object(isolate, vehicleId));
        args.push_back(v8pp::class_<Player>::create_object(isolate, playerId));
        return args;
    });
}

std::string Vehicle::ToString() const {
    std::ostringstream ss;
    ss << "Vehicle{ id: " << _id << " }";
    return ss.str();
}

// Vehicle properties are replicated state on the entity; setting one syncs via the DeltaSerializer.

bool Vehicle::GetBeaconLightsOn() {
    auto *v = ResolveVehicle();
    return v && v->data.beaconLightsOn;
}
void Vehicle::SetBeaconLightsOn(bool on) {
    if (auto *v = ResolveVehicle()) v->data.beaconLightsOn = on;
}

Framework::Scripting::Builtins::Color Vehicle::GetColorPrimary() {
    auto *v = ResolveVehicle();
    return Framework::Scripting::Builtins::Color(v ? v->data.colorPrimary : glm::vec4(0));
}
void Vehicle::SetColorPrimary(Framework::Scripting::Builtins::Color color) {
    if (auto *v = ResolveVehicle()) v->data.colorPrimary = color.vec();
}

Framework::Scripting::Builtins::Color Vehicle::GetColorSecondary() {
    auto *v = ResolveVehicle();
    return Framework::Scripting::Builtins::Color(v ? v->data.colorSecondary : glm::vec4(0));
}
void Vehicle::SetColorSecondary(Framework::Scripting::Builtins::Color color) {
    if (auto *v = ResolveVehicle()) v->data.colorSecondary = color.vec();
}

float Vehicle::GetDirt() {
    auto *v = ResolveVehicle();
    return v ? v->data.dirt : 0.0f;
}
void Vehicle::SetDirt(float dirt) {
    if (auto *v = ResolveVehicle()) v->data.dirt = dirt;
}

bool Vehicle::GetEngineOn() {
    auto *v = ResolveVehicle();
    return v && v->data.engineOn;
}
void Vehicle::SetEngineOn(bool on) {
    if (auto *v = ResolveVehicle()) v->data.engineOn = on;
}

float Vehicle::GetFuel() {
    auto *v = ResolveVehicle();
    return v ? v->data.fuel : 0.0f;
}
void Vehicle::SetFuel(float fuel) {
    if (auto *v = ResolveVehicle()) v->data.fuel = fuel;
}

std::string Vehicle::GetLicensePlate() {
    auto *v = ResolveVehicle();
    return v ? std::string(v->data.licensePlate) : "";
}
void Vehicle::SetLicensePlate(std::string plate) {
    auto *v = ResolveVehicle();
    if (!v) return;
    const size_t copyLength = std::min<size_t>(Shared::Constants::VEHICLE_LICENSE_PLATE_MAX_LENGTH - 1, plate.length());
    std::memcpy(v->data.licensePlate, plate.c_str(), copyLength);
    v->data.licensePlate[copyLength] = '\0';
}

int Vehicle::GetLockState() {
    auto *v = ResolveVehicle();
    return v ? static_cast<int>(v->data.lockState) : 0;
}
void Vehicle::SetLockState(int lockState) {
    if (lockState < 0 || lockState > 2) {
        Framework::Logging::GetLogger("Scripting")->warn("Invalid lockState value: {}", lockState);
        return;
    }
    if (auto *v = ResolveVehicle()) v->data.lockState = static_cast<Shared::Modules::VehicleSync::LockState>(lockState);
}

bool Vehicle::GetRadioOn() {
    auto *v = ResolveVehicle();
    return v && v->data.radioOn;
}
void Vehicle::SetRadioOn(bool on) {
    if (auto *v = ResolveVehicle()) v->data.radioOn = on;
}

int Vehicle::GetRadioStationId() {
    auto *v = ResolveVehicle();
    return v ? v->data.radioStationId : 0;
}
void Vehicle::SetRadioStationId(int id) {
    if (auto *v = ResolveVehicle()) v->data.radioStationId = id;
}

Framework::Scripting::Builtins::Color Vehicle::GetRimColor() {
    auto *v = ResolveVehicle();
    return Framework::Scripting::Builtins::Color(v ? v->data.rimColor : glm::vec4(0));
}
void Vehicle::SetRimColor(Framework::Scripting::Builtins::Color color) {
    if (auto *v = ResolveVehicle()) v->data.rimColor = color.vec();
}

float Vehicle::GetRust() {
    auto *v = ResolveVehicle();
    return v ? v->data.rust : 0.0f;
}
void Vehicle::SetRust(float rust) {
    if (auto *v = ResolveVehicle()) v->data.rust = rust;
}

bool Vehicle::GetSirenOn() {
    auto *v = ResolveVehicle();
    return v && v->data.sirenOn;
}
void Vehicle::SetSirenOn(bool on) {
    if (auto *v = ResolveVehicle()) v->data.sirenOn = on;
}

Framework::Scripting::Builtins::Color Vehicle::GetTireColor() {
    auto *v = ResolveVehicle();
    return Framework::Scripting::Builtins::Color(v ? v->data.tireColor : glm::vec4(0));
}
void Vehicle::SetTireColor(Framework::Scripting::Builtins::Color color) {
    if (auto *v = ResolveVehicle()) v->data.tireColor = color.vec();
}

Framework::Scripting::Builtins::Color Vehicle::GetWindowTint() {
    auto *v = ResolveVehicle();
    return Framework::Scripting::Builtins::Color(v ? v->data.windowTint : glm::vec4(0));
}
void Vehicle::SetWindowTint(Framework::Scripting::Builtins::Color color) {
    if (auto *v = ResolveVehicle()) v->data.windowTint = color.vec();
}

v8pp::class_<Vehicle> &Vehicle::GetClass(v8::Isolate *isolate) {
    if (!_class) {
        _class = std::make_unique<v8pp::class_<Vehicle>>(isolate);
        _class->auto_wrap_objects(true);
        _class->inherit<Entity>()
            .ctor<uint64_t>()
            .function("toString", &Vehicle::ToString);

        auto protoTemplate = _class->class_function_template()->PrototypeTemplate();

        RegisterBoolProp<&Vehicle::GetBeaconLightsOn, &Vehicle::SetBeaconLightsOn>(isolate, protoTemplate, "beaconLightsOn");
        RegisterColorProp<&Vehicle::GetColorPrimary, &Vehicle::SetColorPrimary>(isolate, protoTemplate, "colorPrimary");
        RegisterColorProp<&Vehicle::GetColorSecondary, &Vehicle::SetColorSecondary>(isolate, protoTemplate, "colorSecondary");
        RegisterFloatProp<&Vehicle::GetDirt, &Vehicle::SetDirt>(isolate, protoTemplate, "dirt");
        RegisterBoolProp<&Vehicle::GetEngineOn, &Vehicle::SetEngineOn>(isolate, protoTemplate, "engineOn");
        RegisterFloatProp<&Vehicle::GetFuel, &Vehicle::SetFuel>(isolate, protoTemplate, "fuel");
        RegisterIntProp<&Vehicle::GetLockState, &Vehicle::SetLockState>(isolate, protoTemplate, "lockState");
        RegisterBoolProp<&Vehicle::GetRadioOn, &Vehicle::SetRadioOn>(isolate, protoTemplate, "radioOn");
        RegisterIntProp<&Vehicle::GetRadioStationId, &Vehicle::SetRadioStationId>(isolate, protoTemplate, "radioStationId");
        RegisterColorProp<&Vehicle::GetRimColor, &Vehicle::SetRimColor>(isolate, protoTemplate, "rimColor");
        RegisterFloatProp<&Vehicle::GetRust, &Vehicle::SetRust>(isolate, protoTemplate, "rust");
        RegisterBoolProp<&Vehicle::GetSirenOn, &Vehicle::SetSirenOn>(isolate, protoTemplate, "sirenOn");
        RegisterColorProp<&Vehicle::GetTireColor, &Vehicle::SetTireColor>(isolate, protoTemplate, "tireColor");
        RegisterColorProp<&Vehicle::GetWindowTint, &Vehicle::SetWindowTint>(isolate, protoTemplate, "windowTint");

        // licensePlate (string)
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
    }
    return *_class;
}

void Vehicle::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
    v8pp::class_<Vehicle> &cls = GetClass(isolate);
    auto ctx                   = isolate->GetCurrentContext();
    global->Set(ctx, v8pp::to_v8(isolate, "Vehicle"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
}

} // namespace MafiaMP::Scripting
