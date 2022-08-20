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

#include "../shared/modules/human_sync.hpp"
#include "../shared/messages/human/human_despawn.h"
#include "../shared/messages/human/human_self_update.h"
#include "../shared/messages/human/human_spawn.h"
#include "../shared/messages/human/human_update.h"

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

        struct IsHuman {
            [[maybe_unused]] char _unused;
        };

        Human(flecs::world &world) {
            world.module<Human>();

            world.component<Tracking>();
            world.component<LocalPlayer>();
            world.component<Interpolated>();
            world.component<IsHuman>();

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

        static inline void Create(flecs::entity e, uint64_t spawnProfile) {
            auto info = Core::gApplication->GetEntityFactory()->RequestHuman(spawnProfile);
            auto trackingData = e.get_mut<Core::Modules::Human::Tracking>();
            trackingData->info = info;
            trackingData->human = nullptr;

            auto interp = e.get_mut<Interpolated>();
            interp->interpolator.GetPosition()->SetCompensationFactor(1.5f);

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

            e.add<IsHuman>();
        }

        static inline void Update(flecs::entity e) {
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
                interp->interpolator.GetPosition()->SetTargetValue({ humanPos.x, humanPos.y, humanPos.z }, tr->pos, MafiaMP::Core::gApplication->GetTickInterval());
                interp->interpolator.GetRotation()->SetTargetValue({humanRot.w,humanRot.x, humanRot.y, humanRot.z}, tr->rot, MafiaMP::Core::gApplication->GetTickInterval());
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

        static inline void Remove(flecs::entity e) {
            auto trackingData = e.get_mut<Core::Modules::Human::Tracking>();
            if (!trackingData || e.get<LocalPlayer>() != nullptr) {
                return;
            }

            Core::gApplication->GetEntityFactory()->ReturnEntity(trackingData->info);
            trackingData->info = nullptr;
        }

        static inline void SetupMessages(Application *app) {
            const auto net = app->GetNetworkingEngine()->GetNetworkClient();
            net->RegisterMessage<Shared::Messages::Human::HumanSpawn>(Shared::Messages::ModMessages::MOD_HUMAN_SPAWN, [app](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanSpawn *msg) {
                auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
                if (!e.is_alive()) {
                    return;
                }

                // Setup tracking info
                Create(e, msg->GetSpawnProfile());

                // Setup other components
                e.add<Shared::Modules::HumanSync::UpdateData>();

                // set up client updates (NPC streaming)
                // TODO disabled for now, we don't really need to stream NPCs atm
    #if 0
                const auto es = e.get_mut<Framework::World::Modules::Base::Streamable>();
                es->modEvents.clientUpdateProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                    Shared::Messages::Human::HumanClientUpdate humanUpdate;
                    humanUpdate.FromParameters(e.id());
                    // set up sync data
                    peer->Send(humanUpdate, guid);
                    return true;
                };
    #endif
            });
            net->RegisterMessage<Shared::Messages::Human::HumanDespawn>(Shared::Messages::ModMessages::MOD_HUMAN_DESPAWN, [app](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanDespawn *msg) {
                const auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
                if (!e.is_alive()) {
                    return;
                }

                Remove(e);
            });
            net->RegisterMessage<Shared::Messages::Human::HumanUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_UPDATE, [app](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanUpdate *msg) {
                const auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
                if (!e.is_alive()) {
                    return;
                }

                auto updateData                   = e.get_mut<Shared::Modules::HumanSync::UpdateData>();
                updateData->_charStateHandlerType = msg->GetCharStateHandlerType();
                updateData->_healthPercent        = msg->GetHealthPercent();
                updateData->_isSprinting          = msg->IsSprinting();
                updateData->_isStalking           = msg->IsStalking();
                updateData->_moveMode             = msg->GetMoveMode();
                updateData->_sprintSpeed          = msg->GetSprintSpeed();

                Update(e);
            });
            net->RegisterMessage<Shared::Messages::Human::HumanSelfUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_SELF_UPDATE, [app](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanSelfUpdate *msg) {
                const auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
                if (!e.is_alive()) {
                    return;
                }

                const auto trackingData = e.get<Core::Modules::Human::Tracking>();
                if (!trackingData) {
                    return;
                }

                // update actor data
            });
        }
    };
} // namespace MafiaMP::Core::Modules
