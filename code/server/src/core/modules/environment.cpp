#include "environment.h"

namespace MafiaMP::Core::Modules {
    Environment::Environment(flecs::world& world) {
        world.module<Environment>();
        world.component<Weather>();
    }
}
