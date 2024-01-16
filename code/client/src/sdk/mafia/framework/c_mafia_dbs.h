#pragma once

#include "c_vehicles_database.h"

#include "../../ue/c_weak_ptr.h"
#include "../../ue/sys/sodb/c_sys_odb.h"

namespace SDK {
    namespace mafia::framework {
        class C_MafiaDBs {
          public:
            void GetTablesDatabase(ue::sys::sodb::C_SysODB *);

            ue::C_WeakPtr<mafia::framework::C_VehiclesDatabase> GetVehiclesDatabase();
        };

        static C_MafiaDBs *GetMafiaDBs();
    }; // namespace mafia::framework
};     // namespace SDK
