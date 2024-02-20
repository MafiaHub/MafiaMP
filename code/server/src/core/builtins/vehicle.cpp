#include "vehicle.h"
#include "player.h"

#include "scripting/engines/node/engine.h"
#include "shared/game_rpc/vehicle/vehicle_setprops.h"
#include "shared/modules/vehicle_sync.hpp"

namespace MafiaMP::Scripting {
    v8::Local<v8::Object> Vehicle::WrapVehicle(Framework::Scripting::Engines::Node::Engine *engine, flecs::entity e) {
        return v8pp::class_<Scripting::Vehicle>::create_object(engine->GetIsolate(), e.id());
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

    v8::Local<v8::Object> Vehicle::GetColorPrimary() {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto colorRGB = Framework::Scripting::Engines::Node::Builtins::ColorRGB::FromVec4(vehData->colorPrimary);
        return v8pp::class_<Framework::Scripting::Engines::Node::Builtins::ColorRGB>::create_object(v8::Isolate::GetCurrent(), colorRGB.GetR(), colorRGB.GetG(), colorRGB.GetB());
    }

    void Vehicle::SetColorPrimary(Framework::Scripting::Engines::Node::Builtins::ColorRGB rgb) {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto vec4Color = glm::vec4(rgb.GetFloatR(), rgb.GetFloatG(), rgb.GetFloatB(), 1.0f);

        vehData->colorPrimary = vec4Color;
        MafiaMP::Shared::RPC::VehicleSetProps msg {};
        msg.colorPrimary = vec4Color;
        FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
    }

    v8::Local<v8::Object> Vehicle::GetColorSecondary() {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto colorRGB = Framework::Scripting::Engines::Node::Builtins::ColorRGB::FromVec4(vehData->colorSecondary);
        return v8pp::class_<Framework::Scripting::Engines::Node::Builtins::ColorRGB>::create_object(v8::Isolate::GetCurrent(), colorRGB.GetR(), colorRGB.GetG(), colorRGB.GetB());
    }

    void Vehicle::SetColorSecondary(Framework::Scripting::Engines::Node::Builtins::ColorRGB rgb) {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto vec4Color = glm::vec4(rgb.GetFloatR(), rgb.GetFloatG(), rgb.GetFloatB(), 1.0f);

        vehData->colorSecondary = vec4Color;
        MafiaMP::Shared::RPC::VehicleSetProps msg {};
        msg.colorSecondary = vec4Color;
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
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
        ::memcpy(vehData->licensePlate, plate.c_str(), std::min(sizeof(vehData->licensePlate), plate.length()));
        MafiaMP::Shared::RPC::VehicleSetProps msg {};
        msg.licensePlate = plate.c_str();
        FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
    }

    Shared::Modules::VehicleSync::LockState Vehicle::GetLockState() {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
        return vehData->lockState;
    }

    void Vehicle::SetLockState(Shared::Modules::VehicleSync::LockState lockState) {
        auto vehData       = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();
        vehData->lockState = lockState;
        MafiaMP::Shared::RPC::VehicleSetProps msg {};
        msg.lockState = lockState;
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

    v8::Local<v8::Object> Vehicle::GetRimColor() {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto colorRGB = Framework::Scripting::Engines::Node::Builtins::ColorRGB::FromVec4(vehData->rimColor);
        return v8pp::class_<Framework::Scripting::Engines::Node::Builtins::ColorRGB>::create_object(v8::Isolate::GetCurrent(), colorRGB.GetR(), colorRGB.GetG(), colorRGB.GetB());
    }

    void Vehicle::SetRimColor(Framework::Scripting::Engines::Node::Builtins::ColorRGB rgb) {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto vec4Color = glm::vec4(rgb.GetFloatR(), rgb.GetFloatG(), rgb.GetFloatB(), 1.0f);

        vehData->rimColor = vec4Color;
        MafiaMP::Shared::RPC::VehicleSetProps msg {};
        msg.rimColor = vec4Color;
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

    v8::Local<v8::Object> Vehicle::GetTireColor() {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto colorRGB = Framework::Scripting::Engines::Node::Builtins::ColorRGB::FromVec4(vehData->tireColor);
        return v8pp::class_<Framework::Scripting::Engines::Node::Builtins::ColorRGB>::create_object(v8::Isolate::GetCurrent(), colorRGB.GetR(), colorRGB.GetG(), colorRGB.GetB());
    }

    void Vehicle::SetTireColor(Framework::Scripting::Engines::Node::Builtins::ColorRGB rgb) {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto vec4Color = glm::vec4(rgb.GetFloatR(), rgb.GetFloatG(), rgb.GetFloatB(), 1.0f);

        vehData->tireColor = vec4Color;
        MafiaMP::Shared::RPC::VehicleSetProps msg {};
        msg.tireColor = vec4Color;
        FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
    }

    v8::Local<v8::Object> Vehicle::GetWindowTint() {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto colorRGBA = Framework::Scripting::Engines::Node::Builtins::ColorRGBA::FromVec4(vehData->windowTint);
        return v8pp::class_<Framework::Scripting::Engines::Node::Builtins::ColorRGBA>::create_object(v8::Isolate::GetCurrent(), colorRGBA.GetR(), colorRGBA.GetG(), colorRGBA.GetB(), colorRGBA.GetA());
    }

    void Vehicle::SetWindowTint(Framework::Scripting::Engines::Node::Builtins::ColorRGBA rgba) {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto vec4Color = glm::vec4(rgba.GetFloatR(), rgba.GetFloatG(), rgba.GetFloatB(), rgba.GetFloatA());

        vehData->windowTint = vec4Color;
        MafiaMP::Shared::RPC::VehicleSetProps msg {};
        msg.windowTint = vec4Color;
        FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
    }

    void Vehicle::Register(v8::Isolate *isolate, v8pp::module *rootModule) {
        if (!rootModule) {
            return;
        }

        v8pp::class_<Vehicle> cls(isolate);
        cls.inherit<Framework::Integrations::Scripting::Entity>();

        cls.function("getBeaconLightsOn", &Vehicle::GetBeaconLightsOn);
        cls.function("getColorPrimary", &Vehicle::GetColorPrimary);
        cls.function("getColorSecondary", &Vehicle::GetColorSecondary);
        cls.function("getDirt", &Vehicle::GetDirt);
        cls.function("getEngineOn", &Vehicle::GetEngineOn);
        cls.function("getFuel", &Vehicle::GetFuel);
        cls.function("getLicensePlate", &Vehicle::GetLicensePlate);
        cls.function("getLockState", &Vehicle::GetLockState);
        cls.function("getRadioOn", &Vehicle::GetRadioOn);
        cls.function("getRadioStationId", &Vehicle::GetRadioStationId);
        cls.function("getRimColor", &Vehicle::GetRimColor);
        cls.function("getRust", &Vehicle::GetRust);
        cls.function("getSirenOn", &Vehicle::GetSirenOn);
        cls.function("getTireColor", &Vehicle::GetTireColor);
        cls.function("getWindowTint", &Vehicle::GetWindowTint);

        cls.function("setBeaconLightsOn", &Vehicle::SetBeaconLightsOn);
        cls.function("setColorPrimary", &Vehicle::SetColorPrimary);
        cls.function("setColorSecondary", &Vehicle::SetColorSecondary);
        cls.function("setDirt", &Vehicle::SetDirt);
        cls.function("setEngineOn", &Vehicle::SetEngineOn);
        cls.function("setFuel", &Vehicle::SetFuel);
        cls.function("setLicensePlate", &Vehicle::SetLicensePlate);
        cls.function("setLockState", &Vehicle::SetLockState);
        cls.function("setRadioOn", &Vehicle::SetRadioOn);
        cls.function("setRadioStationId", &Vehicle::SetRadioStationId);
        cls.function("setRimColor", &Vehicle::SetRimColor);
        cls.function("setRust", &Vehicle::SetRust);
        cls.function("setSirenOn", &Vehicle::SetSirenOn);
        cls.function("setTireColor", &Vehicle::SetTireColor);
        cls.function("setWindowTint", &Vehicle::SetWindowTint);

        rootModule->class_("Vehicle", cls);
    }
} // namespace MafiaMP::Scripting
