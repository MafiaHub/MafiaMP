#pragma once

#include <cstdint>

namespace MafiaMP::Shared::Entities {
    // Registers all MafiaMP entity types with the framework EntityFactory. Must be called once on
    // both the client and the server before connecting, so each side can construct replicas of the
    // same types from their wire type id.
    void RegisterEntities();

    uint32_t HumanTypeId();
    uint32_t VehicleTypeId();
} // namespace MafiaMP::Shared::Entities
