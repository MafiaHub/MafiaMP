#include "c_mafia_dbs.h"

#include "sdk/patterns.h"

namespace SDK {
    namespace mafia::framework {
        SDK::ue::C_WeakPtr<SDK::mafia::framework::C_VehiclesDatabase> C_MafiaDBs::GetVehiclesDatabase() {
            ue::C_WeakPtr<mafia::framework::C_VehiclesDatabase> database;
            return hook::this_call<ue::C_WeakPtr<mafia::framework::C_VehiclesDatabase> &>(gPatterns.C_MafiaDBs__GetVehiclesDatabase, this, database);
        }

        static C_MafiaDBs *GetMafiaDBs() {
            return hook::call<C_MafiaDBs *>(hook::get_opcode_address("E8 ? ? ? ? 48 8D 55 87 48 8B C8"));
        }
    }; // namespace mafia::framework
};     // namespace SDK
