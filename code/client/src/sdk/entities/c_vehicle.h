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
        char pad_0000[1052];
#else
        char pad_0000[1036];
#endif

        // 0x268 is VTABLE for radio interface

        ue::sys::math::C_Vector m_Velocity;                                // 0x040C
        ue::sys::math::C_Vector m_AngularVelocity;                         // 0x0418
        char pad_0424[1444];                                               // 0x0424
        float m_fBreak;                                                    // 0x09C8
        char pad_09CC[4];                                                  // 0x09CC
        float m_fHandBreak;                                                // 0x09D0
        float m_fPower;                                                    // 0x09D4
        char pad_09D8[76];                                                 // 0x09D8
        float m_fSteer;                                                    // 0x0A24 - 0x0A28 // 2596 - 2600 // 4
        char pad_0A88[96];                                                 // 0xA28 - 0x0A88 // 2600 - 2696 // 96
        bool m_bHorn;                                                      // 0x0A88 - 0x0A89 // 2696 - 2697 // 1
        char pad_0AF0[103];                                                // 0x0A89 - 0x0AF0 // 2697 - 2800 // 103
        float m_fBrakeHealth1;                                             // 0x0AF0 - 0x0AF4 // 2800 - 2804 // 4
        float m_fBrakeHealth2;                                             // 0x0AF4 - 0x0AF8 // 2804 - 2808 // 4
        char pad_0BB0[184];                                                // 0x0AF8 - 0x0BB0 // 2808 - 2992 // 184
        ue::sys::math::C_Vector4 m_MaterialColor;                          // 0x0BB0 - 0x0BC0 // 2992 - 3008 // 16
        ue::sys::math::C_Vector4 m_MaterialColor1;                         // 0x0BC0 - 0x0BD0 // 3008 - 3024 // 16
        ue::sys::math::C_Vector4 m_WindowTint;                             // 0x0BD0 - 0x0BE0 // 3024 - 3040 // 16
        ue::sys::math::C_Vector4 m_WheelColor;                             // 0x0BE0 - 0x0BF0 // 3040 - 3056 // 16
        ue::sys::math::C_Vector4 m_TireColor;                              // 0x0BF0 - 0x0C00 // 3056 - 3072 // 16
        char pad_0C08[8];                                                  // 0x0C00 - 0x0C08 // 3072 - 3080 // 8
        ue::sys::math::C_Vector4 m_InteriorColors[5];                      // 0x0C08 - 0x0C58 // 3080 - 3160 // 80 (5*16)
        char pad_0C60[8];                                                  // 0x0C58 - 0x0C60 // 3160 - 3168 // 8
        float m_Dirty;                                                     // 0x0C60 - 0x0C64 // 3168 - 3172 // 4
        float m_Rust;                                                      // 0x0C64 - 0x0C68 // 3172 - 3176 // 4
        char pad_14F8[2192];                                               // 0x0C68 - 0x14F8 // 3176 - 5368 // 2192
        SDK::ue::game::audio::radio::C_RadioSound *m_RadioSound = nullptr; // 0x14F8 (NB: IDA says 0x1290 is the offset... yet the game says 0x14F8?
    };
} // namespace SDK
