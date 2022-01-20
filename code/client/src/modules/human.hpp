#pragma once

#include <flecs/flecs.h>

#include "../sdk/c_game.h"
#include "../sdk/entities/c_car.h"
#include "../sdk/entities/c_player_2.h"
#include "../sdk/entities/c_vehicle.h"
#include "../sdk/entities/human/c_human_weapon_controller.h"
#include "../game/streaming/entity_tracking_info.h"

#include <world/modules/base.hpp>

namespace MafiaMP::Core::Modules {
    struct Human {
        struct Tracking {
            SDK::C_Human2 *human;
            Game::Streaming::EntityTrackingInfo *info;
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

        static inline void CreateHuman(flecs::entity e, uint64_t spawnProfile) {
            auto info = Core::gApplication->GetEntityFactory()->RequestHuman(spawnProfile);

            const auto OnHumanRequestFinish = [&](Game::Streaming::EntityTrackingInfo *info, bool success) {
                if (success) {
                    auto human = reinterpret_cast<SDK::C_Human2 *>(info->GetEntity());
                    if (!human) {
                        return;
                    }
                    human->GameInit();
                    human->Activate();

                    const auto ent = info->GetNetworkEntity();
                    const auto tr = ent.get<Framework::World::Modules::Base::Transform>();
                    SDK::ue::sys::math::C_Vector newPos = { tr->pos.x, tr->pos.y, tr->pos.z };
                    SDK::ue::sys::math::C_Quat newRot   = { tr->rot.x, tr->rot.y, tr->rot.z, tr->rot.w };
                    SDK::ue::sys::math::C_Matrix transform = {};
                    transform.Identity();
                    transform.SetRot(newRot);
                    transform.SetPos(newPos);
                    human->SetTransform(transform);

                    auto trackingData = ent.get_mut<Core::Modules::Human::Tracking>();
                    trackingData->human = human;
                }
            };

            const auto OnHumanReturned = [&](Game::Streaming::EntityTrackingInfo *info, bool wasCreated) {
                if (!info) {
                    return;
                }
                auto human = reinterpret_cast<SDK::C_Human2 *>(info->GetEntity());
                if (wasCreated && human) {
                    human->Deactivate();
                    human->GameDone();
                    human->Release();
                }
            };

            // setup tracking callbacks
            info->SetRequestFinishCallback(OnHumanRequestFinish);
            info->SetReturnCallback(OnHumanReturned);
            info->SetNetworkEntity(e);

            auto trackingData = e.get_mut<Core::Modules::Human::Tracking>();
            trackingData->info = info;
        }

        static inline void UpdateHuman(flecs::entity e) {
            const auto trackingData = e.get<Core::Modules::Human::Tracking>();
            if (!trackingData && trackingData->info) {
                return;
            }

            // update actor data
            const auto tr = e.get<Framework::World::Modules::Base::Transform>();
            SDK::ue::sys::math::C_Vector newPos = { tr->pos.x, tr->pos.y, tr->pos.z };
            SDK::ue::sys::math::C_Quat newRot   = { tr->rot.x, tr->rot.y, tr->rot.z, tr->rot.w };
            SDK::ue::sys::math::C_Matrix transform = {};
            transform.Identity();
            transform.SetRot(newRot);
            transform.SetPos(newPos);
            trackingData->human->SetTransform(transform);
        }

        static inline void RemoveHuman(flecs::entity e) {
            auto trackingData = e.get_mut<Core::Modules::Human::Tracking>();
            if (!trackingData || e.get<LocalPlayer>() != nullptr) {
                return;
            }

            Core::gApplication->GetEntityFactory()->ReturnEntity(trackingData->info);
            trackingData->info = nullptr;
        }
    };
} // namespace MafiaMP::Core::Modules
