#include "c_mafia_dbs.h"

#include "sdk/patterns.h"

namespace SDK {
    namespace mafia::framework {

        SDK::ue::C_WeakPtr<SDK::mafia::framework::C_VehiclesDatabase> C_MafiaDBs::GetVehiclesDatabase() {
            ue::C_WeakPtr<mafia::framework::C_VehiclesDatabase> database;
            return hook::this_call<ue::C_WeakPtr<mafia::framework::C_VehiclesDatabase> &>(gPatterns.C_MafiaDBS__GetVehiclesDatabase, this, database);
        }
    }; // namespace mafia::framework
};     // namespace SDK
