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

void Vehicle::EventVehiclePlayerEnter(flecs::entity vehicle, flecs::entity player, int seatIndex) {
    Framework::Logging::GetLogger("Scripting")->debug("Vehicle {} player {} enter seat {}", vehicle.id(), player.id(), seatIndex);

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

    auto vehicleObj = Vehicle::GetClass(isolate).import_external(isolate, new Vehicle(vehicle));
    auto playerObj  = Player::GetClass(isolate).import_external(isolate, new Player(player));

    std::vector<v8::Local<v8::Value>> args;
    args.push_back(vehicleObj);
    args.push_back(playerObj);
    args.push_back(v8::Integer::New(isolate, seatIndex));

    resourceManager->GetEvents().EmitReserved(isolate, context, "vehiclePlayerEnter", args);
}

void Vehicle::EventVehiclePlayerLeave(flecs::entity vehicle, flecs::entity player) {
    Framework::Logging::GetLogger("Scripting")->debug("Vehicle {} player {} leave", vehicle.id(), player.id());

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

    auto vehicleObj = Vehicle::GetClass(isolate).import_external(isolate, new Vehicle(vehicle));
    auto playerObj  = Player::GetClass(isolate).import_external(isolate, new Player(player));

    std::vector<v8::Local<v8::Value>> args;
    args.push_back(vehicleObj);
    args.push_back(playerObj);

    resourceManager->GetEvents().EmitReserved(isolate, context, "vehiclePlayerLeave", args);
}

std::string Vehicle::ToString() const {
    std::ostringstream ss;
    ss << "Vehicle{ id: " << _ent.id() << " }";
    return ss.str();
}

bool Vehicle::GetBeaconLightsOn() {
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
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
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
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
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
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
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
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
    auto syncData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    return syncData->engineOn;
}

void Vehicle::SetEngineOn(bool on) {
    auto vehData      = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->engineOn = on;
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.engineOn = on;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

float Vehicle::GetFuel() {
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
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
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
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
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    return static_cast<int>(vehData->lockState);
}

void Vehicle::SetLockState(int lockState) {
    auto vehData       = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->lockState = static_cast<Shared::Modules::VehicleSync::LockState>(lockState);
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.lockState = vehData->lockState;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

bool Vehicle::GetRadioOn() {
    auto syncData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    return syncData->radioOn;
}

void Vehicle::SetRadioOn(bool on) {
    auto vehData     = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->radioOn = on;
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.radioOn = on;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

int Vehicle::GetRadioStationId() {
    auto syncData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    return syncData->radioStationId;
}

void Vehicle::SetRadioStationId(int id) {
    auto vehData            = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->radioStationId = id;
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.radioStationId = id;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

Framework::Scripting::Builtins::Color Vehicle::GetRimColor() {
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
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
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
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
    auto syncData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    return syncData->sirenOn;
}

void Vehicle::SetSirenOn(bool on) {
    auto vehData     = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
    vehData->sirenOn = on;
    MafiaMP::Shared::RPC::VehicleSetProps msg {};
    msg.sirenOn = on;
    FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
}

Framework::Scripting::Builtins::Color Vehicle::GetTireColor() {
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
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
    auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
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
        _class->inherit<Entity>()
            .ctor<flecs::entity_t>()
            .set("toString", &Vehicle::ToString)
            .set("getBeaconLightsOn", &Vehicle::GetBeaconLightsOn)
            .set("setBeaconLightsOn", &Vehicle::SetBeaconLightsOn)
            .set("getColorPrimary", &Vehicle::GetColorPrimary)
            .set("setColorPrimary", &Vehicle::SetColorPrimary)
            .set("getColorSecondary", &Vehicle::GetColorSecondary)
            .set("setColorSecondary", &Vehicle::SetColorSecondary)
            .set("getDirt", &Vehicle::GetDirt)
            .set("setDirt", &Vehicle::SetDirt)
            .set("getEngineOn", &Vehicle::GetEngineOn)
            .set("setEngineOn", &Vehicle::SetEngineOn)
            .set("getFuel", &Vehicle::GetFuel)
            .set("setFuel", &Vehicle::SetFuel)
            .set("getLicensePlate", &Vehicle::GetLicensePlate)
            .set("setLicensePlate", &Vehicle::SetLicensePlate)
            .set("getLockState", &Vehicle::GetLockState)
            .set("setLockState", &Vehicle::SetLockState)
            .set("getRadioOn", &Vehicle::GetRadioOn)
            .set("setRadioOn", &Vehicle::SetRadioOn)
            .set("getRadioStationId", &Vehicle::GetRadioStationId)
            .set("setRadioStationId", &Vehicle::SetRadioStationId)
            .set("getRimColor", &Vehicle::GetRimColor)
            .set("setRimColor", &Vehicle::SetRimColor)
            .set("getRust", &Vehicle::GetRust)
            .set("setRust", &Vehicle::SetRust)
            .set("getSirenOn", &Vehicle::GetSirenOn)
            .set("setSirenOn", &Vehicle::SetSirenOn)
            .set("getTireColor", &Vehicle::GetTireColor)
            .set("setTireColor", &Vehicle::SetTireColor)
            .set("getWindowTint", &Vehicle::GetWindowTint)
            .set("setWindowTint", &Vehicle::SetWindowTint);
    }
    return *_class;
}

void Vehicle::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
    v8pp::class_<Vehicle> &cls = GetClass(isolate);
    auto ctx                   = isolate->GetCurrentContext();
    global->Set(ctx, v8pp::to_v8(isolate, "Vehicle"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
}

} // namespace MafiaMP::Scripting
