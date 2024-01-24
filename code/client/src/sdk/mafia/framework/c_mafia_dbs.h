#pragma once

#include "c_vehicles_database.h"

#include "../../patterns.h"

#include "../../ue/c_weak_ptr.h"
#include "../../ue/sys/sodb/c_sys_odb.h"

namespace SDK {
    namespace mafia::framework {
        class C_MafiaDBs {
          public:
            void GetTablesDatabase(ue::sys::sodb::C_SysODB *);

            ue::C_WeakPtr<mafia::framework::C_VehiclesDatabase> GetVehiclesDatabase();
        };

        static C_MafiaDBs *GetMafiaDBs() {
            return hook::call<C_MafiaDBs *>(gPatterns.C_MafiaDBs__GetMafiaDBs);
        }
    }; // namespace mafia::framework
};     // namespace SDK
