#include "c_mafia_dbs.h"

#include "../../patterns.h"

namespace SDK {
    namespace mafia::framework {
        void C_MafiaDBs::GetTablesDatabase(ue::sys::sodb::C_SysODB *odb) {
            hook::this_call(gPatterns.C_MafiaDBs__GetTablesDatabase, this, odb);
        }

        SDK::ue::C_WeakPtr<SDK::mafia::framework::C_VehiclesDatabase> C_MafiaDBs::GetVehiclesDatabase() {
            ue::C_WeakPtr<mafia::framework::C_VehiclesDatabase> database;
            return hook::this_call<ue::C_WeakPtr<mafia::framework::C_VehiclesDatabase> &>(gPatterns.C_MafiaDBs__GetVehiclesDatabase, this, database);
        }

        static C_MafiaDBs *GetMafiaDBs() {
            return hook::call<C_MafiaDBs *>(gPatterns.C_MafiaDBs__GetMafiaDbs);
        }
    }; // namespace mafia::framework
};     // namespace SDK
