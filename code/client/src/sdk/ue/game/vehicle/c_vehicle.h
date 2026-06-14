#pragma once

#include "../../sys/math/c_matrix.h"
#include "../../sys/math/c_vector.h"

#include "../audio/radio/c_radio_sound.h"
#include "../../sys/core/c_scene_object.h"

#include "../../../prefab/c_prefab_init_data.h"

#include "../crashobject/c_deformation.h"
#include "c_synchro_command_manager.h"
#include "c_vehicle_real_light_manager.h"

#include <cstdint>

namespace SDK {
    namespace ue::game::vehicle {
        enum E_VehicleFlags {
            FLAG_INDICATOR_LEFT  = 0x8,
            FLAG_INDICATOR_RIGHT = 0x10,
            FLAG_BEACON_LIGHTS   = 0x40,
            FLAG_TAXI_LIGHTS     = 0x80,
        };

        enum E_VehicleIndicator {
            INDICATOR_RIGHT = 0,
            INDICATOR_LEFT  = 1,
        };

        class C_Vehicle : public ue::game::crashobject::C_Deformation {
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

            bool IsIndicatorLightOn(E_VehicleIndicator indicator) {
                return (m_uAdditionalLightFlags & (indicator == E_VehicleIndicator::INDICATOR_LEFT ? E_VehicleFlags::FLAG_INDICATOR_LEFT : E_VehicleFlags::FLAG_INDICATOR_RIGHT)) != 0;
            }
            void SetIndicatorLightsOn(bool, E_VehicleIndicator);

            void SetReflectorLightsOn(bool front, bool rear);

            void SetBeaconLightsOn(bool on);
            bool GetBeaconLightsOn() {
                return (m_uAdditionalLightFlags & E_VehicleFlags::FLAG_BEACON_LIGHTS) != 0;
            }
            void SetSearchLightsOn(bool on);

            void SetSpeedLimiter(bool on);

            void SetTaxiLightsOn(bool on);
            bool GetTaxiLightsOn() {
                return (m_uAdditionalLightFlags & E_VehicleFlags::FLAG_TAXI_LIGHTS) != 0;
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

            C_VehicleRealLightManager *GetRealLightManager() {
                return m_pRealLightManager;
            }

          private:
            // Inherited: ue::game::crashobject::C_Deformation (0x000 - 0x248)
            // Multiple inheritance interface vtables (0x248 - 0x270)
            void *m_pVtblInterface1;                                           // 0248 - 0250
            void *m_pVtblInterface2;                                           // 0250 - 0258
            void *m_pVtblInterface3;                                           // 0258 - 0260
            void *m_pVtblInterface4;                                           // 0260 - 0268
            void *m_pVtblSoundControl;                                         // 0268 - 0270 (radio control interface)
            C_SynchroCommandManager *m_pSynchroCommandMgr;                     // 0270 - 0278
            char pad0[0xA8];                                                   // 0278 - 0320
            prefab::S_PrefabInitData *m_pPrefabInitData;                       // 0320 - 0328
            char pad1[0xE4];                                                   // 0328 - 040C
            ue::sys::math::C_Vector m_vVelocity;                               // 040C - 0418
            ue::sys::math::C_Vector m_vAngularVelocity;                        // 0418 - 0424
            char pad2[0x5A4];                                                  // 0424 - 09C8
            float m_fBreak;                                                    // 09C8 - 09CC
            char pad3[0x4];                                                    // 09CC - 09D0
            float m_fHandBreak;                                                // 09D0 - 09D4
            float m_fPower;                                                    // 09D4 - 09D8
            char pad4[0x4C];                                                   // 09D8 - 0A24
            float m_fSteer;                                                    // 0A24 - 0A28
            char pad5[0x60];                                                   // 0A28 - 0A88
            bool m_bHorn;                                                      // 0A88 - 0A89
            char pad6[0x67];                                                   // 0A89 - 0AF0
            float m_fBrakeHealth1;                                             // 0AF0 - 0AF4
            float m_fBrakeHealth2;                                             // 0AF4 - 0AF8
            char pad7[0xB8];                                                   // 0AF8 - 0BB0
            ue::sys::math::C_Vector4 m_vColor1;                                // 0BB0 - 0BC0
            ue::sys::math::C_Vector4 m_vColor2;                                // 0BC0 - 0BD0
            ue::sys::math::C_Vector4 m_vWindowTint;                            // 0BD0 - 0BE0
            ue::sys::math::C_Vector4 m_vWheelColor;                            // 0BE0 - 0BF0
            ue::sys::math::C_Vector4 m_vTireColor;                             // 0BF0 - 0C00
            char pad8[0x8];                                                    // 0C00 - 0C08
            ue::sys::math::C_Vector4 m_vInteriorColors[5];                     // 0C08 - 0C58
            char pad9[0x8];                                                    // 0C58 - 0C60
            float m_fDirty;                                                    // 0C60 - 0C64
            float m_fRust;                                                     // 0C64 - 0C68
            char pad10[0x88];                                                  // 0C68 - 0CF0
            C_VehicleRealLightManager *m_pRealLightManager;                    // 0CF0 - 0CF8
            char pad11[0x8];                                                   // 0CF8 - 0D00
            uint64_t m_uAdditionalLightFlags;                                  // 0D00 - 0D08
            char pad12[0x428];                                                 // 0D08 - 1130
            char m_pSPZText[7];                                                // 1130 - 1137
            char pad13[0x159];                                                 // 1137 - 1290
            ue::game::audio::radio::C_RadioSound *m_pRadioSound;               // 1290 - 1298
        };
    } // namespace ue::game::vehicle
} // namespace SDK
