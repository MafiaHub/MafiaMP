#include "vehicle.h"

#include "core/server.h"
#include "player.h"

#include "shared/constants.h"

#include <integrations/server/scripting/module.h>
#include <logging/logger.h>
#include <scripting/builtins/property.h>
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
} // namespace

Vehicle::Vehicle(uint64_t networkId): Framework::Scripting::Builtins::Entity(networkId) {
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

// Vehicle properties are replicated state on the entity. For an unowned vehicle the change syncs to
// everyone via the DeltaSerializer; for an owned (driven) vehicle MutateData also forces it onto the
// owner, since the server's word is final even over an entity a client is authoritative for.

bool Vehicle::GetBeaconLightsOn() {
    auto *v = ResolveVehicle();
    return v && v->data.beaconLightsOn;
}
void Vehicle::SetBeaconLightsOn(bool on) {
    MutateData([&](auto &data) { data.beaconLightsOn = on; });
}

Framework::Scripting::Builtins::Color Vehicle::GetColorPrimary() {
    auto *v = ResolveVehicle();
    return Framework::Scripting::Builtins::Color(v ? v->data.colorPrimary : glm::vec4(0));
}
void Vehicle::SetColorPrimary(Framework::Scripting::Builtins::Color color) {
    MutateData([&](auto &data) { data.colorPrimary = color.vec(); });
}

Framework::Scripting::Builtins::Color Vehicle::GetColorSecondary() {
    auto *v = ResolveVehicle();
    return Framework::Scripting::Builtins::Color(v ? v->data.colorSecondary : glm::vec4(0));
}
void Vehicle::SetColorSecondary(Framework::Scripting::Builtins::Color color) {
    MutateData([&](auto &data) { data.colorSecondary = color.vec(); });
}

float Vehicle::GetDirt() {
    auto *v = ResolveVehicle();
    return v ? v->data.dirt : 0.0f;
}
void Vehicle::SetDirt(float dirt) {
    MutateData([&](auto &data) { data.dirt = dirt; });
}

bool Vehicle::GetEngineOn() {
    auto *v = ResolveVehicle();
    return v && v->data.engineOn;
}
void Vehicle::SetEngineOn(bool on) {
    MutateData([&](auto &data) { data.engineOn = on; });
}

float Vehicle::GetFuel() {
    auto *v = ResolveVehicle();
    return v ? v->data.fuel : 0.0f;
}
void Vehicle::SetFuel(float fuel) {
    MutateData([&](auto &data) { data.fuel = fuel; });
}

std::string Vehicle::GetLicensePlate() {
    auto *v = ResolveVehicle();
    return v ? std::string(v->data.licensePlate.data()) : "";
}
void Vehicle::SetLicensePlate(std::string plate) {
    MutateData([&](auto &data) {
        const size_t copyLength = std::min<size_t>(Shared::Constants::VEHICLE_LICENSE_PLATE_MAX_LENGTH - 1, plate.length());
        std::memcpy(data.licensePlate.data(), plate.c_str(), copyLength);
        data.licensePlate[copyLength] = '\0';
    });
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
    MutateData([&](auto &data) { data.lockState = static_cast<Shared::Modules::VehicleSync::LockState>(lockState); });
}

bool Vehicle::GetRadioOn() {
    auto *v = ResolveVehicle();
    return v && v->data.radioOn;
}
void Vehicle::SetRadioOn(bool on) {
    MutateData([&](auto &data) { data.radioOn = on; });
}

int Vehicle::GetRadioStationId() {
    auto *v = ResolveVehicle();
    return v ? v->data.radioStationId : 0;
}
void Vehicle::SetRadioStationId(int id) {
    MutateData([&](auto &data) { data.radioStationId = id; });
}

Framework::Scripting::Builtins::Color Vehicle::GetRimColor() {
    auto *v = ResolveVehicle();
    return Framework::Scripting::Builtins::Color(v ? v->data.rimColor : glm::vec4(0));
}
void Vehicle::SetRimColor(Framework::Scripting::Builtins::Color color) {
    MutateData([&](auto &data) { data.rimColor = color.vec(); });
}

float Vehicle::GetRust() {
    auto *v = ResolveVehicle();
    return v ? v->data.rust : 0.0f;
}
void Vehicle::SetRust(float rust) {
    MutateData([&](auto &data) { data.rust = rust; });
}

bool Vehicle::GetSirenOn() {
    auto *v = ResolveVehicle();
    return v && v->data.sirenOn;
}
void Vehicle::SetSirenOn(bool on) {
    MutateData([&](auto &data) { data.sirenOn = on; });
}

Framework::Scripting::Builtins::Color Vehicle::GetTireColor() {
    auto *v = ResolveVehicle();
    return Framework::Scripting::Builtins::Color(v ? v->data.tireColor : glm::vec4(0));
}
void Vehicle::SetTireColor(Framework::Scripting::Builtins::Color color) {
    MutateData([&](auto &data) { data.tireColor = color.vec(); });
}

Framework::Scripting::Builtins::Color Vehicle::GetWindowTint() {
    auto *v = ResolveVehicle();
    return Framework::Scripting::Builtins::Color(v ? v->data.windowTint : glm::vec4(0));
}
void Vehicle::SetWindowTint(Framework::Scripting::Builtins::Color color) {
    MutateData([&](auto &data) { data.windowTint = color.vec(); });
}

v8pp::class_<Vehicle> &Vehicle::GetClass(v8::Isolate *isolate) {
    if (!_class) {
        _class = std::make_unique<v8pp::class_<Vehicle>>(isolate);
        _class->auto_wrap_objects(true);
        _class->inherit<Framework::Scripting::Builtins::Entity>()
            .ctor<uint64_t>()
            .function("toString", &Vehicle::ToString);

        auto protoTemplate = _class->class_function_template()->PrototypeTemplate();

        using namespace Framework::Scripting::Builtins;

        RegisterProperty<Vehicle, &Vehicle::GetBeaconLightsOn, &Vehicle::SetBeaconLightsOn>(isolate, protoTemplate, "beaconLightsOn");
        RegisterObjectProperty<Vehicle, &Vehicle::GetColorPrimary, &Vehicle::SetColorPrimary>(isolate, protoTemplate, "colorPrimary");
        RegisterObjectProperty<Vehicle, &Vehicle::GetColorSecondary, &Vehicle::SetColorSecondary>(isolate, protoTemplate, "colorSecondary");
        RegisterProperty<Vehicle, &Vehicle::GetDirt, &Vehicle::SetDirt>(isolate, protoTemplate, "dirt");
        RegisterProperty<Vehicle, &Vehicle::GetEngineOn, &Vehicle::SetEngineOn>(isolate, protoTemplate, "engineOn");
        RegisterProperty<Vehicle, &Vehicle::GetFuel, &Vehicle::SetFuel>(isolate, protoTemplate, "fuel");
        RegisterProperty<Vehicle, &Vehicle::GetLicensePlate, &Vehicle::SetLicensePlate>(isolate, protoTemplate, "licensePlate");
        RegisterProperty<Vehicle, &Vehicle::GetLockState, &Vehicle::SetLockState>(isolate, protoTemplate, "lockState");
        RegisterProperty<Vehicle, &Vehicle::GetRadioOn, &Vehicle::SetRadioOn>(isolate, protoTemplate, "radioOn");
        RegisterProperty<Vehicle, &Vehicle::GetRadioStationId, &Vehicle::SetRadioStationId>(isolate, protoTemplate, "radioStationId");
        RegisterObjectProperty<Vehicle, &Vehicle::GetRimColor, &Vehicle::SetRimColor>(isolate, protoTemplate, "rimColor");
        RegisterProperty<Vehicle, &Vehicle::GetRust, &Vehicle::SetRust>(isolate, protoTemplate, "rust");
        RegisterProperty<Vehicle, &Vehicle::GetSirenOn, &Vehicle::SetSirenOn>(isolate, protoTemplate, "sirenOn");
        RegisterObjectProperty<Vehicle, &Vehicle::GetTireColor, &Vehicle::SetTireColor>(isolate, protoTemplate, "tireColor");
        RegisterObjectProperty<Vehicle, &Vehicle::GetWindowTint, &Vehicle::SetWindowTint>(isolate, protoTemplate, "windowTint");
    }
    return *_class;
}

void Vehicle::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
    v8pp::class_<Vehicle> &cls = GetClass(isolate);
    auto ctx                   = isolate->GetCurrentContext();
    global->Set(ctx, v8pp::to_v8(isolate, "Vehicle"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
}

} // namespace MafiaMP::Scripting
