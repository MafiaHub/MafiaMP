#include "c_vehicles_database.h"

#include "../../patterns.h"

namespace SDK {
    namespace mafia::framework {
        uint32_t C_VehiclesDatabase::GetVehiclesCount() {
            return hook::this_call<uint32_t>(gPatterns.C_VehiclesDatabase__GetVehiclesCount, this);
        }

        const C_VehiclesDatabase::TItemAccessorConst &C_VehiclesDatabase::GetVehicleByIndex(uint32_t index) {
            TItemAccessorConst TempA2; // the game function outputs the result in a2 and return, so we can get away with just one
            return hook::this_call<const TItemAccessorConst &>(gPatterns.C_VehiclesDatabase__GetVehicleByIndex, this, &TempA2, index);
        }

        const C_VehiclesDatabase::TItemAccessorConst &C_VehiclesDatabase::GetVehicleByID(uint32_t index) {
            TItemAccessorConst TempA2; // the game function outputs the result in a2 and return, so we can get away with just one
            return hook::this_call<const TItemAccessorConst &>(gPatterns.C_VehiclesDatabase__GetVehicleByID, this, &TempA2, index);
        }

        const C_VehiclesDatabase::TItemAccessorConst &C_VehiclesDatabase::GetVehicleByModel(uint64_t hash) {
            TItemAccessorConst TempA2; // the game function outputs the result in a2 and return, so we can get away with just one
            return hook::this_call<const TItemAccessorConst &>(gPatterns.C_VehiclesDatabase__GetVehicleByModel, this, &TempA2, hash);
        }
    } // namespace mafia::framework
} // namespace SDK
