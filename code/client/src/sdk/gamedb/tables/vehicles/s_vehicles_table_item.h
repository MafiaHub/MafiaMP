#pragma once

#include "sdk/mafia/tables/table_defines.h"
#include "sdk/mafia/tables/vehicle/vehicle_defines.h"
#include "sdk/mafia/traffic/traffic_defines.h"

#include <utils/bitops.h>

#include <stdint.h>

namespace SDK {
    namespace gamedb::tables::vehicles {
        struct S_VehiclesTableItem {
          public:

            bool HasCommonFlags(const mafia::traffic::E_TrafficCommonFlags flags) const {
                return BIT_HAS(m_CommonFlags, flags);
            }

            bool HasVehicleFlags(const mafia::traffic::E_TrafficVehicleFlags flags) const {
                return BIT_HAS(m_VehicleFlags, flags);
            }

            bool HasVehicleLookFlags(const mafia::traffic::E_TrafficVehicleLookFlags flags) const {
                return BIT_HAS(m_VehicleLookFlags, flags);
            }

            bool HasVehicleFunctionFlags(const mafia::tables::vehicle::E_VehiclesTableFunctionFlags flags) const {
                return BIT_HAS(m_VehicleFunctionFlags, flags);
            }

            // Accessors for all properties within the table item;
            // Note that there is no accessor for flags, use public API to query instead
            uint32_t GetID() const {
                return m_ID;
            }

            const char *GetModelName() const {
                return &m_ModelName[0];
            }

            const char *GetSoundVehicleSwitch() const {
                return &m_SoundVehicleSwitch[0];
            }

            mafia::tables::E_RadioStation GetRadioStation() const {
                return m_RadioStation;
            };

            float GetRadioSoundQuality() const {
                return m_RadioSoundQuality;
            }

            uint32_t GetTexID() const {
                return m_TexID;
            }

            uint64_t GetTexHash() const {
                return m_TexHash;
            }

            const char *GetBrandNameUI() const {
                return &m_BrandNameUI[0];
            }

            const char *GetModelNameUI() const {
                return &m_ModelNameUI[0];
            }

            const char *GetLogoNameUI() const {
                return &m_LogoNameUI[0];
            }

            uint32_t GetStealCoefficient() const {
                return m_StealCoefficient;
            }

            uint32_t GetPrice() const {
                return m_Price;
            }

            float GetMinDirt() const {
                return m_MinDirt;
            }

            float GetMaxDirt() const {
                return m_MaxDirt;
            }

            float GetMinRust() const {
                return m_MinDirt;
            }

            float GetMaxRust() const {
                return m_MaxRust;
            }

            mafia::tables::vehicle::E_VehicleRaceClass GetRaceClass() const {
                return m_RaceClass;
            }

            float GetTrunkDoorOffsetX() const {
                return m_TrunkDockOffsetX;
            }

            float GetTrunkDoorOffsetY() const {
                return m_TrunkDockOffsetY;
            }

          private:
            uint32_t m_ID = 0;
            mafia::traffic::E_TrafficCommonFlags m_CommonFlags;
            mafia::traffic::E_TrafficVehicleFlags m_VehicleFlags;
            mafia::traffic::E_TrafficVehicleLookFlags m_VehicleLookFlags;
            mafia::tables::vehicle::E_VehiclesTableFunctionFlags m_VehicleFunctionFlags;
            char m_ModelName[32];
            char m_SoundVehicleSwitch[32];
            mafia::tables::E_RadioStation m_RadioStation;
            float m_RadioSoundQuality = 100.0f;
            uint32_t m_TexID          = 0;
            uint64_t m_TexHash        = 0;
            char m_BrandNameUI[32];
            char m_ModelNameUI[32];
            char m_LogoNameUI[32];
            uint32_t m_StealCoefficient = 0;
            uint32_t m_Price            = 0;
            float m_MinDirt             = 0.0f;
            float m_MinRust             = 0.0f;
            float m_MaxDirt             = 0.0f;
            float m_MaxRust             = 0.0f;
            mafia::tables::vehicle::E_VehicleRaceClass m_RaceClass;
            float m_TrunkDockOffsetX = 0.0f;
            float m_TrunkDockOffsetY = 0.0f;
        };
    } // namespace gamedb::tables::vehicles
} // namespace SDK
