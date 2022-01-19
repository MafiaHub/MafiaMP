#pragma once

#include <flecs/flecs.h>

#include "../sdk/c_game.h"
#include "../sdk/entities/c_car.h"
#include "../sdk/entities/c_player_2.h"
#include "../sdk/entities/c_vehicle.h"
#include "../sdk/entities/human/c_human_weapon_controller.h"

#include <world/modules/base.hpp>

namespace MafiaMP::Core::Modules {
    struct Human {
        struct Tracking {
            SDK::C_Human2 *human;
        };

        struct LocalPlayer {
            char _unused;
        };

        Human(flecs::world &world) {
            world.module<Human>();

            world.component<Tracking>();
            world.component<LocalPlayer>();

            world.system<Tracking, LocalPlayer, Framework::World::Modules::Base::Transform>("UpdateLocalPlayer").each([](flecs::entity e, Tracking &tracking, LocalPlayer &lp, Framework::World::Modules::Base::Transform &tr) {
                if (tracking.human) {
                    SDK::ue::sys::math::C_Vector newPos = tracking.human->GetPos();
                    SDK::ue::sys::math::C_Quat newRot   = tracking.human->GetRot();
                    tr.pos                              = {newPos.x, newPos.y, newPos.z};
                    tr.rot                              = {newRot.w, newRot.x, newRot.y, newRot.z};
                }
            });
        }
    };
} // namespace MafiaMP::Core::Modules
