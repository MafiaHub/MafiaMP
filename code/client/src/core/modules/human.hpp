#pragma once

#include <flecs/flecs.h>

#include "../sdk/c_game.h"
#include "../sdk/entities/c_car.h"
#include "../sdk/entities/c_player_2.h"
#include "../sdk/entities/c_vehicle.h"
#include "../sdk/entities/human/c_human_weapon_controller.h"
#include "../sdk/entities/human/c_human_script.h"

#include "../game/streaming/entity_tracking_info.h"
#include "../game/helpers/human.h"
#include "../game/overrides/character_controller.h"

#include "../shared/modules/human_sync.hpp"

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
            char _unused;
        };

        Human(flecs::world &world) {
            world.module<Human>();

            world.component<Tracking>();
            world.component<LocalPlayer>();
            world.component<Interpolated>();

            world.system<Tracking, Shared::Modules::HumanSync::UpdateData, LocalPlayer, Framework::World::Modules::Base::Transform>("UpdateLocalPlayer").each([](flecs::entity e, Tracking &tracking, Shared::Modules::HumanSync::UpdateData &metadata, LocalPlayer &lp, Framework::World::Modules::Base::Transform &tr) {
                if (tracking.human) {
                    SDK::ue::sys::math::C_Vector newPos = tracking.human->GetPos();
                    SDK::ue::sys::math::C_Quat newRot   = tracking.human->GetRot();
                    tr.pos                              = {newPos.x, newPos.y, newPos.z};
                    tr.rot                              = {newRot.w, newRot.x, newRot.y, newRot.z};

                    // Store the required metadata for onfoot sync
                    auto charController            = tracking.human->GetCharacterController();
                    metadata._healthPercent = MafiaMP::Game::Helpers::Human::GetHealthPercent(tracking.human);
                    metadata._charStateHandlerType = charController->GetCurrentStateHandler()->GetStateHandlerType();
                    metadata._isStalking           = charController->IsStalkMove();
                    metadata._isSprinting          = charController->IsSprinting();
                    metadata._sprintSpeed          = charController->GetSprintMoveSpeed();

                    // Current state-specific sync data
                    switch (metadata._charStateHandlerType) {
                    case SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_MOVE: {
                        SDK::E_HumanMoveMode hmm = charController->GetHumanMoveMode();
                        uint8_t moveMode        = hmm != SDK::E_HumanMoveMode::E_HMM_NONE ? static_cast<uint8_t>(hmm) : (uint8_t)-1;

                        metadata._moveMode = moveMode;

                    } break;

                    case SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR: {
                        //TODO: implement
                    } break;
                    }
                }
            });

            world.system<Tracking, Interpolated>("UpdateRemoteHuman").each([](flecs::entity e, Tracking &tracking, Interpolated &interpolated) {
                if (tracking.human && e.get<LocalPlayer>() == nullptr) {
                    const auto humanPos = tracking.human->GetPos();
                    const auto humanRot = tracking.human->GetRot();
                    const auto newPos = interpolated.interpolator.GetPosition()->UpdateTargetValue({humanPos.x, humanPos.y, humanPos.z});
                    const auto newRot = interpolated.interpolator.GetRotation()->UpdateTargetValue({humanRot.w, humanRot.x, humanRot.y, humanRot.z});
                    tracking.human->SetPos({newPos.x, newPos.y, newPos.z});
                    tracking.human->SetRot({newRot.x, newRot.y, newRot.z, newRot.w});
                }
            });
        }

        static inline void CreateHuman(flecs::entity e, uint64_t spawnProfile) {
            auto info = Core::gApplication->GetEntityFactory()->RequestHuman(spawnProfile);
            auto trackingData = e.get_mut<Core::Modules::Human::Tracking>();
            trackingData->info = info;
            trackingData->human = nullptr;

            e.add<Interpolated>();

            const auto OnHumanRequestFinish = [&](Game::Streaming::EntityTrackingInfo *info, bool success) {
                CreateNetCharacterController = false;
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
                    trackingData->charController = reinterpret_cast<MafiaMP::Game::Overrides::CharacterController *>(human->GetCharacterController());
                    assert(MafiaMP::Game::Overrides::CharacterController::IsInstanceOfClass(trackingData->charController));
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
            info->SetBeforeSpawnCallback([&](Game::Streaming::EntityTrackingInfo *) {
                CreateNetCharacterController = true;
            });
            info->SetRequestFinishCallback(OnHumanRequestFinish);
            info->SetReturnCallback(OnHumanReturned);
            info->SetNetworkEntity(e);
        }

        static inline void UpdateHuman(flecs::entity e) {
            const auto trackingData = e.get<Core::Modules::Human::Tracking>();
            if (!trackingData || !trackingData->human) {
                return;
            }

            trackingData->human->GetHumanScript()->SetDemigod(true);
            trackingData->human->GetHumanScript()->SetInvulnerabilityByScript(true);

            // Update basic data
            const auto tr = e.get<Framework::World::Modules::Base::Transform>();
            auto interp = e.get_mut<Interpolated>();
            if (interp) {
                const auto humanPos = trackingData->human->GetPos();
                const auto humanRot = trackingData->human->GetRot();
                interp->interpolator.GetPosition()->SetTargetValue({ humanPos.x, humanPos.y, humanPos.z }, tr->pos, 0.01667f); // todo fetch from tick interval
                interp->interpolator.GetRotation()->SetTargetValue({humanRot.w,humanRot.x, humanRot.y, humanRot.z}, tr->rot, 0.01667f); // todo fetch from tick interval
            } else {
                SDK::ue::sys::math::C_Vector newPos = { tr->pos.x, tr->pos.y, tr->pos.z };
                SDK::ue::sys::math::C_Quat newRot   = { tr->rot.x, tr->rot.y, tr->rot.z, tr->rot.w };
                SDK::ue::sys::math::C_Matrix transform = {};
                transform.Identity();
                transform.SetRot(newRot);
                transform.SetPos(newPos);
                trackingData->human->SetTransform(transform);
            }

            // Update human based data
            const auto updateData = e.get<Shared::Modules::HumanSync::UpdateData>();
            SDK::ue::game::humanai::C_CharacterStateHandler::E_State_Handler_Type desiredStateHandlerType = static_cast<SDK::ue::game::humanai::C_CharacterStateHandler::E_State_Handler_Type>(updateData->_charStateHandlerType);

            // TODO: take care of vehicle sync data
            trackingData->charController->SetDesiredHandlerType(desiredStateHandlerType);
            trackingData->charController->SetStalkMoveOverride(updateData->_isStalking);
            SDK::E_HumanMoveMode hmm = updateData->_moveMode != (uint8_t)-1 ? static_cast<SDK::E_HumanMoveMode>(updateData->_moveMode) : SDK::E_HumanMoveMode::E_HMM_NONE;
            trackingData->charController->SetMoveStateOverride(hmm, updateData->_isSprinting, updateData->_sprintSpeed);
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
