#pragma once

#include <flecs/flecs.h>

//#include "game/overrides/character_controller.h"
//#include "sdk/entities/c_human_2.h"
//
//#include

namespace MafiaMP::Core::Modules {
    struct Human {
        struct Tracking {
            void *_todo;
        };

        struct LocalPlayer {
            char _unused;
        };

        Human(flecs::world &world) {
            world.module<Human>();

            world.component<Tracking>();
            world.component<LocalPlayer>();
        }
    };
} // namespace MafiaMP::Core::Modules
