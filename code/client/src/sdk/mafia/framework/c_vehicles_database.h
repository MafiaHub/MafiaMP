#pragma once

#include "sdk/gamedb/tables/vehicles/s_vehicles_table_item.h"
#include "sdk/ue/sys/sodb/c_database_interface.h"

namespace SDK {
    namespace mafia::framework {

        /**
         * Contains XBin data at runtime; assume to be read-only
         */
        class C_VehiclesDatabase: public ue::sys::sodb::C_DatabaseInterface {
          public:

              uint32_t GetVehiclesCount();
              
              typedef ue::sys::sodb::C_Database::C_AccessorConst<SDK::gamedb::tables::vehicles::S_VehiclesTableItem> TItemAccessorConst;
              const TItemAccessorConst& GetVehicleByIndex(uint32_t index);

              const TItemAccessorConst &GetVehicleByID(uint32_t index);

              // Hash as in model name converted to FNV hash
              const TItemAccessorConst &GetVehicleByModel(uint64_t hash);
        };
    }; // namespace mafia::framework
};     // namespace SDK
