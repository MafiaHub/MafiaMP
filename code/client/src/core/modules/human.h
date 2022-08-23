#pragma once

#include <flecs/flecs.h>

#include "sdk/entities/c_player_2.h"

#include "game/streaming/entity_tracking_info.h"
#include "game/overrides/character_controller.h"

#include "core/application.h"

#include <world/modules/base.hpp>
#include <utils/interpolator.h>

namespace MafiaMP::Core::Modules {
    struct Human {
        struct Tracking {
            SDK::C_Human2 *human                                 = nullptr;
            Game::Overrides::CharacterController *charController = nullptr;
            Game::Streaming::EntityTrackingInfo *info            = nullptr;
        };

        struct Interpolated {
            Framework::Utils::Interpolator interpolator = {};
        };

        struct LocalPlayer {
            [[maybe_unused]] char _unused;
        };

        Human(flecs::world &world);

        static void Create(flecs::entity e, uint64_t spawnProfile);

        static void SetupLocalPlayer(Application *app, flecs::entity e);

        static void Update(flecs::entity e);

        static void Remove(flecs::entity e);

        static void SetupMessages(Application *app);
        
        static flecs::query<Tracking> findAllHumans;
    };
} // namespace MafiaMP::Core::Modules
