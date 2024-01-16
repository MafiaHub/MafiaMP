#pragma once

#include "c_vehicles_database.h"

#include "sdk/ue/c_weak_ptr.h"
#include "sdk/ue/sys/sodb/c_sys_odb.h"

#include "../../patterns.h"

namespace SDK {
    namespace mafia::framework {
        class C_MafiaDBs {
          public:
            void GetTablesDatabase(ue::sys::sodb::C_SysODB *odb) {
                hook::this_call(gPatterns.C_MafiaDBs__GetTablesDatabase, this, odb);
            }

            ue::C_WeakPtr<mafia::framework::C_VehiclesDatabase> GetVehiclesDatabase();
        };

        static C_MafiaDBs *GetMafiaDBs();
    }; // namespace mafia::framework
};     // namespace SDK
