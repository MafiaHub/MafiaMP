#pragma once

#include "core/server.h"
#include <flecs/flecs.h>

namespace MafiaMP::Core::Modules {
    class Human {
      public:
        Human(flecs::world &world);

        static void Create(Framework::Networking::NetworkServer *net, flecs::entity e);

        static void SetupMessages(std::shared_ptr<Framework::World::ServerEngine> srv, Framework::Networking::NetworkServer *net);
    };
} // namespace MafiaMP
