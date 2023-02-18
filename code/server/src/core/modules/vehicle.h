#pragma once

#include "core/server.h"
#include <flecs/flecs.h>

#include "shared/modules/vehicle_sync.hpp"

namespace MafiaMP::Core::Modules {
    class Vehicle {
      public:
        struct CarData {
            uint64_t seats[4]{};
            Shared::Modules::VehicleSync::LockState locked = Shared::Modules::VehicleSync::LockState::Unlocked;
        };

        Vehicle(flecs::world &world);

        static flecs::entity Create(MafiaMP::Server *server);

        static void SetupMessages(std::shared_ptr<Framework::World::ServerEngine> srv, Framework::Networking::NetworkServer *net);
    };
} // namespace MafiaMP::Core::Modules
