#pragma once

#include <flecs/flecs.h>

#include "sdk/entities/c_player_2.h"

#include "game/streaming/entity_tracking_info.h"
#include "game/overrides/character_controller.h"

#include "core/application.h"

#include <world/modules/base.hpp>
#include <utils/interpolator.h>

enum CarEnterStates { STATE_OUTSIDE, STATE_ENTERING, STATE_LEAVING, STATE_INSIDE };

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

        struct HumanData {
            struct CarPassenger {
                CarEnterStates enterState = STATE_OUTSIDE;
                bool enterForced{};
            } carPassenger{};
        };

        Human(flecs::world &world);

        static void Create(flecs::entity e, uint64_t spawnProfile);

        static void SetupLocalPlayer(Application *app, flecs::entity e);

        static void Update(flecs::entity e);

        static void Remove(flecs::entity e);

        static void SetupMessages(Application *app);

        static void UpdateTransform(flecs::entity);

        static flecs::query<Tracking> findAllHumans;

        static flecs::entity GetHumanEntity(SDK::C_Human2 *ptr);

        private:
        static void InitRPCs(Application *app);
    };
} // namespace MafiaMP::Core::Modules
