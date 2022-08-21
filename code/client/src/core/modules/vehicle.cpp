#include <utils/safe_win32.h>
#include "vehicle.hpp"

namespace MafiaMP::Core::Modules {
    flecs::query<Vehicle::Tracking> Vehicle::_findAllVehicles;

    flecs::entity_t Vehicle::GetCarEntity(SDK::C_Car *carPtr) {
        flecs::entity_t carID{};
        _findAllVehicles.each([&carID, carPtr](flecs::entity e, Core::Modules::Vehicle::Tracking &trackingData) {
            if (trackingData.car == carPtr) {
                const auto sid = e.get<Framework::World::Modules::Base::ServerID>();
                carID = sid->id;
            }
        });
        return carID;
    }
} // namespace MafiaMP::Core::Modules
