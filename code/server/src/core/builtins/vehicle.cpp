#include "vehicle.h"

#include "core/server.h"

#include "shared/game_rpc/vehicle/vehicle_setprops.h"
#include "shared/modules/vehicle_sync.hpp"

#include "player.h"

namespace MafiaMP::Scripting {
    void Vehicle::EventVehiclePlayerEnter(flecs::entity vehicle, flecs::entity player, int seatIndex) {
        const auto engine = MafiaMP::Server::GetScriptingEngine();

        auto vehicleObj = Vehicle(vehicle);
        auto playerObj  = Player(player);

        engine->InvokeEvent("onVehiclePlayerEnter", vehicleObj, playerObj, seatIndex);
    }

    void Vehicle::EventVehiclePlayerLeave(flecs::entity vehicle, flecs::entity player) {
        const auto engine = MafiaMP::Server::GetScriptingEngine();

        auto vehicleObj = Vehicle(vehicle);
        auto playerObj  = Player(player);

        engine->InvokeEvent("onVehiclePlayerLeave", vehicleObj, playerObj);
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

    Framework::Scripting::Builtins::ColorRGB Vehicle::GetColorPrimary() {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto colorRGB = Framework::Scripting::Builtins::ColorRGB::FromVec4(vehData->colorPrimary);
        return colorRGB;
    }

    void Vehicle::SetColorPrimary(Framework::Scripting::Builtins::ColorRGB rgb) {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto vec4Color = glm::vec4(rgb.GetFloatR(), rgb.GetFloatG(), rgb.GetFloatB(), 1.0f);

        vehData->colorPrimary = vec4Color;
        MafiaMP::Shared::RPC::VehicleSetProps msg {};
        msg.colorPrimary = vec4Color;
        FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
    }

    Framework::Scripting::Builtins::ColorRGB Vehicle::GetColorSecondary() {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto colorRGB = Framework::Scripting::Builtins::ColorRGB::FromVec4(vehData->colorSecondary);
        return colorRGB;
    }

    void Vehicle::SetColorSecondary(Framework::Scripting::Builtins::ColorRGB rgb) {
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

        size_t copyLength = std::min<size_t>(Shared::Modules::VehicleSync::LICENSE_PLATE_MAX_LENGTH - 1, plate.length());
        std::memcpy(vehData->licensePlate, plate.c_str(), copyLength);

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

    Framework::Scripting::Builtins::ColorRGB Vehicle::GetRimColor() {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto colorRGB = Framework::Scripting::Builtins::ColorRGB::FromVec4(vehData->rimColor);
        return colorRGB;
    }

    void Vehicle::SetRimColor(Framework::Scripting::Builtins::ColorRGB rgb) {
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

    Framework::Scripting::Builtins::ColorRGB Vehicle::GetTireColor() {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto colorRGB = Framework::Scripting::Builtins::ColorRGB::FromVec4(vehData->tireColor);
        return colorRGB;
    }

    void Vehicle::SetTireColor(Framework::Scripting::Builtins::ColorRGB rgb) {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto vec4Color = glm::vec4(rgb.GetFloatR(), rgb.GetFloatG(), rgb.GetFloatB(), 1.0f);

        vehData->tireColor = vec4Color;
        MafiaMP::Shared::RPC::VehicleSetProps msg {};
        msg.tireColor = vec4Color;
        FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
    }

    Framework::Scripting::Builtins::ColorRGBA Vehicle::GetWindowTint() {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto colorRGBA = Framework::Scripting::Builtins::ColorRGBA::FromVec4(vehData->windowTint);
        return colorRGBA;
    }

    void Vehicle::SetWindowTint(Framework::Scripting::Builtins::ColorRGBA rgba) {
        auto vehData = _ent.get_mut<Shared::Modules::VehicleSync::UpdateData>();

        auto vec4Color = glm::vec4(rgba.GetFloatR(), rgba.GetFloatG(), rgba.GetFloatB(), rgba.GetFloatA());

        vehData->windowTint = vec4Color;
        MafiaMP::Shared::RPC::VehicleSetProps msg {};
        msg.windowTint = vec4Color;
        FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::VehicleSetProps, _ent, msg);
    }

    void Vehicle::Register(sol::state &luaEngine) {
        sol::usertype<Vehicle> cls = luaEngine.new_usertype<Vehicle>("Vehicle", sol::constructors<Vehicle(uint64_t)>(), sol::base_classes, sol::bases<Entity>());
        cls["getBeaconLightsOn"]   = &Vehicle::GetBeaconLightsOn;
        cls["setBeaconLightsOn"]   = &Vehicle::SetBeaconLightsOn;
        cls["getColorPrimary"]     = &Vehicle::GetColorPrimary;
        cls["setColorPrimary"]     = &Vehicle::SetColorPrimary;
        cls["getColorSecondary"]   = &Vehicle::GetColorSecondary;
        cls["setColorSecondary"]   = &Vehicle::SetColorSecondary;
        cls["getDirt"]             = &Vehicle::GetDirt;
        cls["setDirt"]             = &Vehicle::SetDirt;
        cls["getEngineOn"]         = &Vehicle::GetEngineOn;
        cls["setEngineOn"]         = &Vehicle::SetEngineOn;
        cls["getFuel"]             = &Vehicle::GetFuel;
        cls["setFuel"]             = &Vehicle::SetFuel;
        cls["getLicensePlate"]     = &Vehicle::GetLicensePlate;
        cls["setLicensePlate"]     = &Vehicle::SetLicensePlate;
        cls["getLockState"]        = &Vehicle::GetLockState;
        cls["setLockState"]        = &Vehicle::SetLockState;
        cls["getRadioOn"]          = &Vehicle::GetRadioOn;
        cls["setRadioOn"]          = &Vehicle::SetRadioOn;
        cls["getRadioStationId"]   = &Vehicle::GetRadioStationId;
        cls["setRadioStationId"]   = &Vehicle::SetRadioStationId;
        cls["getRimColor"]         = &Vehicle::GetRimColor;
        cls["setRimColor"]         = &Vehicle::SetRimColor;
        cls["getRust"]             = &Vehicle::GetRust;
        cls["setRust"]             = &Vehicle::SetRust;
        cls["getSirenOn"]          = &Vehicle::GetSirenOn;
        cls["setSirenOn"]          = &Vehicle::SetSirenOn;
        cls["getTireColor"]        = &Vehicle::GetTireColor;
        cls["setTireColor"]        = &Vehicle::SetTireColor;
        cls["getWindowTint"]       = &Vehicle::GetWindowTint;
        cls["setWindowTint"]       = &Vehicle::SetWindowTint;
    }
} // namespace MafiaMP::Scripting
