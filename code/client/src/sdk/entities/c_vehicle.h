#pragma once

#include "../ue/sys/math/c_matrix.h"
#include "../ue/sys/math/c_vector.h"

#include "../ue/game/audio/radio/c_radio_sound.h"
#include "../ue/sys/core/c_scene_object.h"

#include <cstdint>

namespace SDK {
    enum E_VehicleFlags {
        BEACON_LIGHTS = 0x40,
        TAXI_LIGHTS   = 0x80,
    };

    class C_Vehicle {
      public:
        void AddVehicleFlags(uint64_t);
        void ClearVehicleFlags(uint64_t);

        void SetVehicleMatrix(const ue::sys::math::C_Matrix &transform, ue::sys::core::E_TransformChangeType changeType);

        void SetSPZText(const char *spz, bool unk1);
        const char *GetSPZText();

        void SetBrake(float arg1, bool arg2);
        const float GetBrake() const {
            return m_fBreak;
        }

        void SetHandbrake(float arg1, bool arg2);
        const float GetHandbrake() const {
            return m_fHandBreak;
        }

        void SetEngineOn(bool on, bool arg2);

        void SetPower(float power);
        float GetPower() const {
            return m_fPower;
        }

        void SetSiren(bool on);
        bool IsSiren();

        void SetSteer(float steer);
        float GetSteer() const {
            return m_fSteer;
        }

        void SetHorn(bool on);
        bool GetHorn() const {
            return m_bHorn;
        }

        void SetSpeed(ue::sys::math::C_Vector const &speed, bool arg2, bool arg3);
        const ue::sys::math::C_Vector &GetSpeed() const {
            return m_vVelocity;
        }

        void SetAngularSpeed(ue::sys::math::C_Vector const &speed, bool arg2);
        const ue::sys::math::C_Vector &GetAngularSpeed() const {
            return m_vAngularVelocity;
        }

        void SetVehicleDirty(float level);
        float GetVehicleDirty() const {
            return m_fDirty;
        }

        void SetVehicleRust(float rust);
        float GetVehicleRust() const {
            return m_fRust;
        }

        void SetVehicleColor(ue::sys::math::C_Vector4 const *color1, ue::sys::math::C_Vector4 const *color2, bool metallic);
        void GetVehicleColor(ue::sys::math::C_Vector4 *color1, ue::sys::math::C_Vector4 *color2) const;

        void SetWindowTintColor(const ue::sys::math::C_Vector4 &color);
        const ue::sys::math::C_Vector4 GetWindowTintColor() const {
            return m_vWindowTint;
        }

        void SetWheelColor(ue::sys::math::C_Vector4 const *rimColor, ue::sys::math::C_Vector4 const *tireColor);
        void GetWheelColor(ue::sys::math::C_Vector4 *rimColor, ue::sys::math::C_Vector4 *tireColor) const;

        void SetInteriorColors(ue::sys::math::C_Vector4 const *color1, ue::sys::math::C_Vector4 const *color2, ue::sys::math::C_Vector4 const *color3, ue::sys::math::C_Vector4 const *color4, ue::sys::math::C_Vector4 const *color5);
        void GetInteriorColors(ue::sys::math::C_Vector4 *color1, ue::sys::math::C_Vector4 *color2, ue::sys::math::C_Vector4 *color3, ue::sys::math::C_Vector4 *color4, ue::sys::math::C_Vector4 *color5) const;

        void GetDamageBrakes(float &brake1, float &brake2) const;
        void DamageBrakes(float brake1, float brake2);

        void SetDoorFree(int doorIdx, bool arg2, bool arg3);
        void SetGear(int gear);

        void SetBeaconLightsOn(bool on);
        bool GetBeaconLightsOn() {
            return (m_uLightFlags & E_VehicleFlags::BEACON_LIGHTS) != 0;
        }
        void SetSearchLightsOn(bool on);

        void SetSpeedLimiter(bool on);

        void SetTaxiLightsOn(bool on);
        bool GetTaxiLightsOn() {
            return (m_uLightFlags & E_VehicleFlags::TAXI_LIGHTS) != 0;
        }

        void SetActive(bool active);
        bool IsActive(int arg1 = 0);

        void Damage(bool arg1);

        bool IsAnyLightOn();

        bool IsRadioOn();
        void EnableRadio(bool enable);
        void TurnRadioOn(bool on);
        uint32_t GetRadioStation();
        void ChangeRadioStation(uint32_t stationSelection);

      private:
#ifdef NONSTEAM_SUPPORT
        char pad_base[0x41C];
#else
        char pad_base[0x40C];
#endif

        ue::sys::math::C_Vector m_vVelocity;                               // 040C - 0418
        ue::sys::math::C_Vector m_vAngularVelocity;                        // 0418 - 0424
        char pad0[0x5A4];                                                  // 0424 - 09C8
        float m_fBreak;                                                    // 09C8 - 09CC
        char pad1[0x4];                                                    // 09CC - 09D0
        float m_fHandBreak;                                                // 09D0 - 09D4
        float m_fPower;                                                    // 09D4 - 09D8
        char pad2[0x4C];                                                   // 09D8 - 0A24
        float m_fSteer;                                                    // 0A24 - 0A28
        char pad3[0x60];                                                   // 0A28 - 0A88
        bool m_bHorn;                                                      // 0A88 - 0A89
        char pad4[0x67];                                                   // 0A89 - 0AF0
        float m_fBrakeHealth1;                                             // 0AF0 - 0AF4
        float m_fBrakeHealth2;                                             // 0AF4 - 0AF8
        char pad5[0xB8];                                                   // 0AF8 - 0BB0
        ue::sys::math::C_Vector4 m_vColor1;                                // 0BB0 - 0BC0
        ue::sys::math::C_Vector4 m_vColor2;                                // 0BC0 - 0BD0
        ue::sys::math::C_Vector4 m_vWindowTint;                            // 0BD0 - 0BE0
        ue::sys::math::C_Vector4 m_vWheelColor;                            // 0BE0 - 0BF0
        ue::sys::math::C_Vector4 m_vTireColor;                             // 0BF0 - 0C00
        char pad6[0x8];                                                    // 0C00 - 0C08
        ue::sys::math::C_Vector4 m_vInteriorColors[5];                     // 0C08 - 0C58
        char pad7[0x8];                                                    // 0C58 - 0C60
        float m_fDirty;                                                    // 0C60 - 0C64
        float m_fRust;                                                     // 0C64 - 0C68
        char pad8[0x98];                                                   // 0C68 - 0D00
        uint64_t m_uLightFlags;                                            // 0D00 - 0D08
        char pad9[0x7F0];                                                  // 0D08 - 14F8
        ue::game::audio::radio::C_RadioSound *m_pRadioSound;               // 14F8 - 1500
    };
} // namespace SDK
