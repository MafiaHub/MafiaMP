#pragma once

#include "../ue/sys/math/c_matrix.h"
#include "../ue/sys/math/c_vector.h"

#include "../ue/game/audio/radio/c_radio_sound.h"
#include "../ue/sys/core/c_scene_object.h"

#include <cstdint>

namespace SDK {
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
        bool IsEngineOn() const {
            return false; // TODO: implement me
        }

        void SetPower(float power);
        float GetPower() const {
            return m_fPower;
        }

        void SetSiren(bool on);
        bool GetSiren() const {
            return false; // TODO: implement me
        }

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
            return m_Velocity;
        }

        void SetAngularSpeed(ue::sys::math::C_Vector const &speed, bool arg2);
        const ue::sys::math::C_Vector &GetAngularSpeed() const {
            return m_AngularVelocity;
        }

        void SetVehicleDirty(float level);
        float GetVehicleDirty() const {
            return m_Dirty;
        }

        void SetVehicleRust(float rust);
        float GetVehicleRust() const {
            return m_Rust;
        }

        void SetVehicleColor(ue::sys::math::C_Vector4 const *color1, ue::sys::math::C_Vector4 const *color2, bool metallic);
        void GetVehicleColor(ue::sys::math::C_Vector4 *color1, ue::sys::math::C_Vector4 *color2) const;

        void SetWindowTintColor(const ue::sys::math::C_Vector4 &color);
        const ue::sys::math::C_Vector4 GetWindowTintColor() const {
            return m_WindowTint;
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
            return false; // TODO: implement me
        }
        void SetSearchLightsOn(bool on);

        void SetSpeedLimiter(bool on);

        void SetActive(bool active);
        bool IsActive(int arg1 = 0);

        void Damage(bool arg1);

        bool IsRadioOn();
        void EnableRadio(bool enable);
        void TurnRadioOn(bool on);
        uint32_t GetRadioStation();
        void ChangeRadioStation(uint32_t stationSelection);

      private:
#ifdef NONSTEAM_SUPPORT
        char pad_0000[0x41C];
#else
        char pad_0000[0x40C];
#endif

        // 0x268 is VTABLE for radio interface

        ue::sys::math::C_Vector m_Velocity;                                // 040C - 0418
        ue::sys::math::C_Vector m_AngularVelocity;                         // 0418 - 0424
        char pad_0424[0x5A4];                                              // 0424 - 09C8
        float m_fBreak;                                                    // 09C8 - 09CC
        char pad_09CC[0x4];                                                // 09CC - 09D0
        float m_fHandBreak;                                                // 09D0 - 09D4
        float m_fPower;                                                    // 09D4 - 09D8
        char pad_09D8[0x4C];                                               // 09D8 - 0A24
        float m_fSteer;                                                    // 0A24 - 0A28
        char pad_0A28[0x60];                                               // 0A28 - 0A88
        bool m_bHorn;                                                      // 0A88 - 0A89
        char pad_0A89[0x67];                                               // 0A89 - 0AF0
        float m_fBrakeHealth1;                                             // 0AF0 - 0AF4
        float m_fBrakeHealth2;                                             // 0AF4 - 0AF8
        char pad_0AF8[0xB8];                                               // 0AF8 - 0BB0
        ue::sys::math::C_Vector4 m_MaterialColor;                          // 0BB0 - 0BC0
        ue::sys::math::C_Vector4 m_MaterialColor1;                         // 0BC0 - 0BD0
        ue::sys::math::C_Vector4 m_WindowTint;                             // 0BD0 - 0BE0
        ue::sys::math::C_Vector4 m_WheelColor;                             // 0BE0 - 0BF0
        ue::sys::math::C_Vector4 m_TireColor;                              // 0BF0 - 0C00
        char pad_0C00[0x8];                                                // 0C00 - 0C08
        ue::sys::math::C_Vector4 m_InteriorColors[5];                      // 0C08 - 0C58
        char pad_0C58[0x8];                                                // 0C58 - 0C60
        float m_Dirty;                                                     // 0C60 - 0C64
        float m_Rust;                                                      // 0C64 - 0C68
        char pad_0C68[0x890];                                              // 0C68 - 14F8
        SDK::ue::game::audio::radio::C_RadioSound *m_RadioSound = nullptr; // 14F8 - 1500 (NB: IDA says 0x1290 is the offset... yet the game says 0x14F8)
    };
} // namespace SDK
