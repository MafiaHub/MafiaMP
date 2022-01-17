#pragma once

#include <flecs/flecs.h>

#include "../../../sdk/entities/c_human_2.h"
#include "../../../game/overrides/character_controller.h"
#include "../../../game/streaming/entity_tracking_info.h"


namespace MafiaMP::Core::World::Modules {
    struct HumanEntity {
        struct Tracking {
            SDK::C_Human2 *_human;
            Game::Overrides::CharacterController *_characterController;
            Game::Streaming::EntityTrackingInfo *_trackingInformation;
        };

        HumanEntity(flecs::world &world){
            world.module<HumanEntity>();

            world.component<Tracking>();
        }
    };
}
