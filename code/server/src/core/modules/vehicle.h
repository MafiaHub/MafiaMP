#pragma once

#include "core/server.h"
#include <flecs/flecs.h>

#include "shared/modules/vehicle_sync.hpp"

namespace MafiaMP::Core::Modules {
    class Vehicle {
      public:
        // server only data
        struct CarData {
            uint64_t seats[4] {};
        };

        Vehicle(flecs::world &world);

        static flecs::entity Create(MafiaMP::Server *server);

        static void InitRPCs(std::shared_ptr<Framework::World::ServerEngine> srv, Framework::Networking::NetworkServer *net);
        static void SetupMessages(std::shared_ptr<Framework::World::ServerEngine> srv, Framework::Networking::NetworkServer *net);
    };
} // namespace MafiaMP::Core::Modules
