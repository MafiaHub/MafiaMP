#include "c_vehicle.h"

#include <utils/hooking/hooking.h>

#include "../patterns.h"

namespace SDK {
    void C_Vehicle::AddVehicleFlags(uint64_t flags) {
        hook::this_call(gPatterns.C_Vehicle__AddVehicleFlags, this, flags);
    }

    void C_Vehicle::ClearVehicleFlags(uint64_t flags) {
        hook::this_call(gPatterns.C_Vehicle__ClearVehicleFlags, this, flags);
    }

    void C_Vehicle::SetVehicleMatrix(const ue::sys::math::C_Matrix &transform, ue::sys::core::E_TransformChangeType changeType) {
        hook::this_call(gPatterns.C_Vehicle__SetVehicleMatrix, this, transform, changeType);
    }

    void C_Vehicle::SetSPZText(const char *spz, bool unk1) {
        hook::this_call(gPatterns.C_Vehicle__SetSPZText, this, spz, unk1);
    }

    const char *C_Vehicle::GetSPZText() {
        return hook::this_call<const char *>(gPatterns.C_Vehicle__GetSPZText, this);
    }

    void C_Vehicle::SetBrake(float force, bool arg2) {
        hook::this_call(gPatterns.C_Vehicle__SetBrake, this, force, arg2);
    }

    void C_Vehicle::SetHandbrake(float force, bool arg2) {
        hook::this_call(gPatterns.C_Vehicle__SetHandbrake, this, force, arg2);
    }

    void C_Vehicle::SetEngineOn(bool arg1, bool arg2) {
        hook::this_call(gPatterns.C_Vehicle__SetEngineOn, this, arg1, arg2);
    }

    void C_Vehicle::SetPower(float power) {
        hook::this_call(gPatterns.C_Vehicle__SetPower, this, power);
    }

    void C_Vehicle::SetSiren(bool on) {
        hook::this_call(gPatterns.C_Vehicle__SetSiren, this, on);
    }

    void C_Vehicle::SetSteer(float steer) {
        hook::this_call(gPatterns.C_Vehicle__SetSteer, this, steer);
    }

    void C_Vehicle::SetHorn(bool on) {
        hook::this_call(gPatterns.C_Vehicle__SetHorn, this, on);
    }

    void C_Vehicle::SetSpeed(ue::sys::math::C_Vector const &speed, bool arg2, bool arg3) {
        hook::this_call(gPatterns.C_Vehicle__SetSpeed, this, speed, arg2, arg3);
    }

    void C_Vehicle::SetAngularSpeed(ue::sys::math::C_Vector const &speed, bool arg2) {
        hook::this_call(gPatterns.C_Vehicle__SetAngularSpeed, this, speed, arg2);
    }

    void C_Vehicle::SetVehicleDirty(float level) {
        hook::this_call(gPatterns.C_Vehicle__SetVehicleDirty, this, level);
    }

    void C_Vehicle::SetVehicleRust(float rust) {
        hook::this_call(gPatterns.C_Vehicle__SetVehicleRust, this, rust);
    }

    void C_Vehicle::SetVehicleColor(ue::sys::math::C_Vector4 const *color1, ue::sys::math::C_Vector4 const *color2, bool unk) {
        hook::this_call(gPatterns.C_Vehicle__SetVehicleColor, this, color1, color2, unk);
    }

    void C_Vehicle::GetVehicleColor(ue::sys::math::C_Vector4 *color1, ue::sys::math::C_Vector4 *color2) const {
        *color1 = m_Color1;
        *color2 = m_Color2;
    }

    void C_Vehicle::SetWindowTintColor(const ue::sys::math::C_Vector4 &color) {
        hook::this_call(gPatterns.C_Vehicle__SetWindowTintColor, this, color);
    }

    void C_Vehicle::SetWheelTintColor(const ue::sys::math::C_Vector4 &color) {
        hook::this_call(gPatterns.C_Vehicle__SetWheelTintColor, this, color);
    }

    void C_Vehicle::SetInteriorColors(ue::sys::math::C_Vector4 const *color1, ue::sys::math::C_Vector4 const *color2, ue::sys::math::C_Vector4 const *color3,
                                      ue::sys::math::C_Vector4 const *color4, ue::sys::math::C_Vector4 const *color5) {
        hook::this_call(gPatterns.C_Vehicle__SetInteriorColors, this, color1, color2, color3, color4, color5);
    }

    void C_Vehicle::GetInteriorColors(ue::sys::math::C_Vector4 *color1, ue::sys::math::C_Vector4 *color2, ue::sys::math::C_Vector4 *color3, ue::sys::math::C_Vector4 *color4,
                                      ue::sys::math::C_Vector4 *color5) const {
        *color1 = m_InteriorColors[0];
        *color2 = m_InteriorColors[1];
        *color3 = m_InteriorColors[2];
        *color4 = m_InteriorColors[3];
        *color5 = m_InteriorColors[4];
    }

    void C_Vehicle::DamageBrakes(float brake1, float brake2) {
        hook::this_call(gPatterns.C_Vehicle__DamageBreaks, this, brake1, brake2);
    }

    void C_Vehicle::GetDamageBrakes(float &brake1, float &brake2) const {
        brake1 = (1.0f - m_fBrakeHealth1);
        brake2 = (1.0f - m_fBrakeHealth2);
    }

    void C_Vehicle::SetDoorFree(int doorIdx, bool arg2, bool arg3) {
        hook::this_call(gPatterns.C_Vehicle__SetDoorFree, this, doorIdx, arg2, arg3);
    }

    void C_Vehicle::SetGear(int gear) {
        hook::this_call(gPatterns.C_Vehicle__SetGear, this, gear);
    }

    void C_Vehicle::SetBeaconLightsOn(bool on) {
        hook::this_call(gPatterns.C_Vehicle__SetBeaconLightsOn, this, on);
    }

    void C_Vehicle::SetSearchLightsOn(bool on) {
        hook::this_call(gPatterns.C_Vehicle__SetSearchLightsOn, this, on);
    }

    void C_Vehicle::SetSpeedLimiter(bool on) {
        hook::this_call(gPatterns.C_Vehicle__SetSpeedLimit, this, on);
    }

    void C_Vehicle::SetActive(bool state) {
        hook::this_call(gPatterns.C_Vehicle__SetActive, this, state);
    }

    bool C_Vehicle::IsActive(int arg1) {
        return hook::this_call<bool>(gPatterns.C_Vehicle__IsActive, this, arg1);
    }

    void C_Vehicle::EnableRadio(bool enable) {
        hook::this_call(gPatterns.C_Vehicle__EnableRadio, this, enable);
    }

    void C_Vehicle::TurnRadioOn(bool on) {
        hook::this_call(gPatterns.C_Vehicle__TurnRadioOn, this, on);
    }

    void C_Vehicle::Damage(bool arg1) {
        hook::this_call(gPatterns.C_Vehicle__Damage, this, arg1);
    }
}
