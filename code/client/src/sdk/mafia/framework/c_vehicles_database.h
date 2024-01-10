#pragma once

#include "sdk/mafia/tables/table_defines.h"
#include "sdk/mafia/tables/vehicle/vehicle_defines.h"
#include "sdk/mafia/traffic/traffic_defines.h"
#include "sdk/ue/sys/sodb/c_database_interface.h"

#include <stdint.h>

namespace SDK {
    namespace mafia::framework {

        // NB: This should actually reside in the namespace: gamedb::tables::vehicles
        struct S_VehiclesTableItem {
            uint32_t m_ID                   = 0;
            mafia::traffic::E_TrafficCommonFlags m_CommonFlags;
            mafia::traffic::E_TrafficVehicleFlags m_VehicleFlags;
            mafia::traffic::E_TrafficVehicleLookFlags m_VehicleLookFlags;
            mafia::tables::vehicle::E_VehiclesTableFunctionFlags m_VehicleFunctionFlags;
            char m_ModelName[32];
            char m_SoundVehicleSwitch[32];
            mafia::tables::E_RadioStation m_RadioStation;
            float m_RadioSoundQuality       = 100.0f;
            uint32_t m_TexID                = 0;
            uint64_t m_TexHash              = 0;
            char m_BrandNameUI[32];
            char m_ModelNameUI[32];
            char m_LogoNameUI[32];
            uint32_t m_StealCoefficient     = 0;
            uint32_t m_Price                = 0;
            float m_MinDirt                 = 0.0f;
            float m_MinRust                 = 0.0f;
            float m_MaxDirt                 = 0.0f;
            float m_MaxRust                 = 0.0f;
            mafia::tables::vehicle::E_VehicleRaceClass m_RaceClass;
            float m_TrunkDockOffsetX        = 0.0f;
            float m_TrunkDockOffsetY        = 0.0f;
        };

        /**
         * Contains XBin data at runtime; assume to be read-only
         */
        class C_VehiclesDatabase: public ue::sys::sodb::C_DatabaseInterface {
          public:

              uint32_t GetVehiclesCount();
              
              typedef ue::sys::sodb::C_Database::C_AccessorConst<S_VehiclesTableItem> TItemAccessorConst;
              const TItemAccessorConst& GetVehicleByIndex(uint32_t index);

              const TItemAccessorConst &GetVehicleByID(uint32_t index);

              // Hash as in model name converted to FNV hash
              const TItemAccessorConst &GetVehicleByModel(uint64_t hash);
        };
    }; // namespace mafia::framework
};     // namespace SDK
