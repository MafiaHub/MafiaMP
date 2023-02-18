#include <utils/safe_win32.h>

#include "human.h"

#include <flecs/flecs.h>

#include "sdk/c_game.h"
#include "sdk/entities/c_car.h"
#include "sdk/entities/c_player_2.h"
#include "sdk/entities/c_vehicle.h"
#include "sdk/entities/human/c_human_script.h"
#include "sdk/entities/human/c_human_weapon_controller.h"
#include "sdk/wrappers/c_human_2_car_wrapper.h"

#include "game/helpers/controls.h"
#include "game/helpers/human.h"
#include "game/overrides/character_controller.h"
#include "game/streaming/entity_tracking_info.h"

#include "shared/game_rpc/human/human_shoot.h"
#include "shared/game_rpc/human/human_reload.h"
#include "shared/game_rpc/human/human_changeskin.h"

#include "vehicle.h"
#include <world/modules/base.hpp>

#include "shared/messages/human/human_despawn.h"
#include "shared/messages/human/human_self_update.h"
#include "shared/messages/human/human_spawn.h"
#include "shared/messages/human/human_update.h"
#include "shared/modules/human_sync.hpp"
#include "shared/modules/mod.hpp"

namespace MafiaMP::Core::Modules {

    flecs::query<Human::Tracking> Human::findAllHumans;

    Human::Human(flecs::world &world) {
        world.module<Human>();

        world.component<Tracking>();
        world.component<LocalPlayer>();
        world.component<Interpolated>();
        world.component<HumanData>();

        findAllHumans = world.query_builder<Human::Tracking>().build();

        world.system<Tracking, Shared::Modules::HumanSync::UpdateData, LocalPlayer, Framework::World::Modules::Base::Transform>("UpdateLocalPlayer")
            .each([](flecs::entity e, Tracking &tracking, Shared::Modules::HumanSync::UpdateData &metadata, LocalPlayer &lp, Framework::World::Modules::Base::Transform &tr) {
                if (tracking.human) {
                    SDK::ue::sys::math::C_Vector newPos = tracking.human->GetPos();
                    SDK::ue::sys::math::C_Quat newRot   = tracking.human->GetRot();
                    tr.pos                              = {newPos.x, newPos.y, newPos.z};
                    tr.rot                              = {newRot.w, newRot.x, newRot.y, newRot.z};

                    // Store the required metadata for onfoot sync
                    auto charController            = tracking.human->GetCharacterController();
                    metadata._healthPercent        = MafiaMP::Game::Helpers::Human::GetHealthPercent(tracking.human);
                    metadata._charStateHandlerType = charController->GetCurrentStateHandler()->GetStateHandlerType();
                    metadata._isStalking           = charController->IsStalkMove();
                    metadata._isSprinting          = charController->IsSprinting();
                    metadata._sprintSpeed          = charController->GetSprintMoveSpeed();

                    // Current state-specific sync data
                    switch (metadata._charStateHandlerType) {
                    case SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_MOVE: {
                        SDK::E_HumanMoveMode hmm = charController->GetHumanMoveMode();
                        uint8_t moveMode         = hmm != SDK::E_HumanMoveMode::E_HMM_NONE ? static_cast<uint8_t>(hmm) : (uint8_t)-1;

                        metadata._moveMode = moveMode;

                    } break;

                    case SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR: {
                        auto human2CarWrapper = charController->GetCarHandler()->GetHuman2CarWrapper();
                        // printf("id: %d\n", charController->GetCarHandler()->GetCarState());
                        if (human2CarWrapper && charController->GetCarHandler()->GetCarState() == 2) /* entering in progress */ {
                            auto *car        = (SDK::C_Car *)tracking.human->GetOwner();
                            const auto carId = Core::Modules::Vehicle::GetCarEntity(car);
                            if (carId.is_valid()) {
                                metadata.carPassenger = {carId.id(), (int)human2CarWrapper->GetSeatID(tracking.human)};
                            }
                        }
                        else if (human2CarWrapper && charController->GetCarHandler()->GetCarState() == 8) /* leaving in progress */ {
                            metadata.carPassenger = {};
                        }
                    } break;
                    }

                    if (metadata._charStateHandlerType != SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR) {
                        // not in a vehicle, so make sure data is reset
                        metadata.carPassenger = {};
                    }
                }
            });

        world.system<Tracking, Interpolated>("UpdateRemoteHuman").each([](flecs::entity e, Tracking &tracking, Interpolated &interpolated) {
            if (tracking.human && e.get<LocalPlayer>() == nullptr) {
                auto updateData = e.get_mut<Shared::Modules::HumanSync::UpdateData>();
                auto humanData  = e.get_mut<HumanData>();
                if (tracking.charController->GetCurrentStateHandlerType() != SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR) {
                    if (humanData->carPassenger.enterState == STATE_ENTERING) {
                        const auto carEnt = Core::gApplication->GetWorldEngine()->GetEntityByServerID(updateData->carPassenger.carId);
                        if (carEnt.is_valid()) {
                            const auto carTracking = carEnt.get<Core::Modules::Vehicle::Tracking>();
                            if (carTracking) {
                                if (Game::Helpers::Human::PutIntoCar(tracking.charController, carTracking->car, updateData->carPassenger.seatId, humanData->carPassenger.enterForced)) {
                                    humanData->carPassenger.enterState  = STATE_INSIDE;
                                    humanData->carPassenger.enterForced = false;
                                }
                            }
                        }
                    }
                    else if (!SDK::ue::game::humanai::C_CharacterStateHandler::IsVehicleStateHandlerType(tracking.charController->GetCurrentStateHandlerType())) {
                        const auto humanPos = tracking.human->GetPos();
                        const auto humanRot = tracking.human->GetRot();
                        const auto newPos   = interpolated.interpolator.GetPosition()->UpdateTargetValue({humanPos.x, humanPos.y, humanPos.z});
                        const auto newRot   = interpolated.interpolator.GetRotation()->UpdateTargetValue({humanRot.w, humanRot.x, humanRot.y, humanRot.z});
                        tracking.human->SetPos({newPos.x, newPos.y, newPos.z});
                        tracking.human->SetRot({newRot.x, newRot.y, newRot.z, newRot.w});
                    }
                }
            }
        });
    }

    void Human::Create(flecs::entity e, uint64_t spawnProfile) {
        auto info           = Core::gApplication->GetEntityFactory()->RequestHuman(spawnProfile);
        auto trackingData   = e.get_mut<Core::Modules::Human::Tracking>();
        trackingData->info  = info;
        trackingData->human = nullptr;

        auto interp = e.get_mut<Interpolated>();
        interp->interpolator.GetPosition()->SetCompensationFactor(1.5f);

        e.add<HumanData>();
        e.set<Shared::Modules::Mod::EntityKind>({Shared::Modules::Mod::MOD_PLAYER});

        const auto OnHumanRequestFinish = [](Game::Streaming::EntityTrackingInfo *info, bool success) {
            CreateNetCharacterController = false;
            if (success) {
                auto human = reinterpret_cast<SDK::C_Human2 *>(info->GetEntity());
                if (!human) {
                    return;
                }
                human->GameInit();
                human->Activate();

                const auto ent                         = info->GetNetworkEntity();
                const auto tr                          = ent.get<Framework::World::Modules::Base::Transform>();
                SDK::ue::sys::math::C_Vector newPos    = {tr->pos.x, tr->pos.y, tr->pos.z};
                SDK::ue::sys::math::C_Quat newRot      = {tr->rot.x, tr->rot.y, tr->rot.z, tr->rot.w};
                SDK::ue::sys::math::C_Matrix transform = {};
                transform.Identity();
                transform.SetRot(newRot);
                transform.SetPos(newPos);
                human->SetTransform(transform);

                auto trackingData            = ent.get_mut<Core::Modules::Human::Tracking>();
                trackingData->human          = human;
                trackingData->charController = reinterpret_cast<MafiaMP::Game::Overrides::CharacterController *>(human->GetCharacterController());
                assert(MafiaMP::Game::Overrides::CharacterController::IsInstanceOfClass(trackingData->charController));

                // TODO(DavoSK): remove
                Game::Helpers::Human::AddWeapon(human, 85, 200);
                Game::Helpers::Human::AddWeapon(human, 3, 200);
                Game::Helpers::Human::AddWeapon(human, 13, 200);
            }
        };

        const auto OnHumanReturned = [](Game::Streaming::EntityTrackingInfo *info, bool wasCreated) {
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

    void Human::SetupLocalPlayer(Application *, flecs::entity e) {
        auto trackingData   = e.get_mut<Core::Modules::Human::Tracking>();
        trackingData->human = Game::Helpers::Controls::GetLocalPlayer();
        trackingData->info  = nullptr;

        e.add<Shared::Modules::HumanSync::UpdateData>();
        e.add<Core::Modules::Human::LocalPlayer>();
        e.add<HumanData>();
        e.set<Shared::Modules::Mod::EntityKind>({Shared::Modules::Mod::MOD_PLAYER});

        // TODO(DavoSK): remove
        Game::Helpers::Human::AddWeapon(trackingData->human, 85, 200);
        Game::Helpers::Human::AddWeapon(trackingData->human, 3, 200);

        const auto es            = e.get_mut<Framework::World::Modules::Base::Streamable>();
        es->modEvents.updateProc = [](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
            const auto updateData = e.get<Shared::Modules::HumanSync::UpdateData>();

            Shared::Messages::Human::HumanUpdate humanUpdate {};
            humanUpdate.SetServerID(Framework::World::ClientEngine::GetServerID(e));
            humanUpdate.SetData(*updateData);
            peer->Send(humanUpdate, guid);
            return true;
        };
    }

    void Human::Update(flecs::entity e) {
        const auto trackingData = e.get<Core::Modules::Human::Tracking>();
        if (!trackingData || !trackingData->human) {
            return;
        }

        trackingData->human->GetHumanScript()->SetDemigod(true);
        trackingData->human->GetHumanScript()->SetInvulnerabilityByScript(true);

        auto updateData                    = e.get_mut<Shared::Modules::HumanSync::UpdateData>();
        auto humanData                     = e.get_mut<HumanData>();
        const auto desiredStateHandlerType = static_cast<SDK::ue::game::humanai::C_CharacterStateHandler::E_State_Handler_Type>(updateData->_charStateHandlerType);

        // exit vehicle if we're no longer a passenger
        if (updateData->carPassenger.carId == 0 && humanData->carPassenger.enterState == STATE_INSIDE) {
            humanData->carPassenger.enterState = STATE_LEAVING;
            if (Game::Helpers::Human::RemoveFromCar(trackingData->charController, (SDK::C_Car *)trackingData->human->GetOwner(), false)) {
                humanData->carPassenger.enterState = STATE_OUTSIDE;
            }
        }

        if (SDK::ue::game::humanai::C_CharacterStateHandler::IsVehicleStateHandlerType(desiredStateHandlerType)) {
            trackingData->charController->SetDesiredHandlerType(SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_NONE);

            if (desiredStateHandlerType == SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR) {
                if (trackingData->charController->GetCurrentStateHandlerType() != SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR) {
                    if (updateData->carPassenger.carId > 0 && humanData->carPassenger.enterState == STATE_OUTSIDE) {
                        humanData->carPassenger.enterState = STATE_ENTERING;
                    }
                }
            }

            return;
        }

        // Update basic data
        const auto tr = e.get<Framework::World::Modules::Base::Transform>();
        auto interp   = e.get_mut<Interpolated>();
        if (interp) {
            const auto humanPos = trackingData->human->GetPos();
            const auto humanRot = trackingData->human->GetRot();
            interp->interpolator.GetPosition()->SetTargetValue({humanPos.x, humanPos.y, humanPos.z}, tr->pos, MafiaMP::Core::gApplication->GetTickInterval());
            interp->interpolator.GetRotation()->SetTargetValue({humanRot.w, humanRot.x, humanRot.y, humanRot.z}, tr->rot, MafiaMP::Core::gApplication->GetTickInterval());
        }
        else {
            SDK::ue::sys::math::C_Vector newPos    = {tr->pos.x, tr->pos.y, tr->pos.z};
            SDK::ue::sys::math::C_Quat newRot      = {tr->rot.x, tr->rot.y, tr->rot.z, tr->rot.w};
            SDK::ue::sys::math::C_Matrix transform = {};
            transform.Identity();
            transform.SetRot(newRot);
            transform.SetPos(newPos);
            trackingData->human->SetTransform(transform);
        }

        // Update human based data
        trackingData->charController->SetDesiredHandlerType(desiredStateHandlerType);
        trackingData->charController->SetStalkMoveOverride(updateData->_isStalking);
        const auto hmm = updateData->_moveMode != (uint8_t)-1 ? static_cast<SDK::E_HumanMoveMode>(updateData->_moveMode) : SDK::E_HumanMoveMode::E_HMM_NONE;
        trackingData->charController->SetMoveStateOverride(hmm, updateData->_isSprinting, updateData->_sprintSpeed);

        // weapon sync
        const auto wepController = trackingData->human->GetHumanWeaponController();

        // NOTE(DavoSK): we are doin those two inside hook, since it could fight with game
        wepController->SetAiming(updateData->weaponData.isAiming);
        wepController->SetFirePressedFlag(updateData->weaponData.isFiring);

        if (wepController->GetRightHandWeaponID() != updateData->weaponData.currentWeaponId) {
            wepController->DoWeaponSelectByItemId(updateData->weaponData.currentWeaponId, true);
        }
    }

    void Human::Remove(flecs::entity e) {
        auto trackingData = e.get_mut<Core::Modules::Human::Tracking>();
        if (!trackingData || e.get<LocalPlayer>() != nullptr) {
            return;
        }

        Core::gApplication->GetEntityFactory()->ReturnEntity(trackingData->info);
        trackingData->info = nullptr;
    }

    void Human::SetupMessages(Application *app) {
        const auto net = app->GetNetworkingEngine()->GetNetworkClient();
        net->RegisterMessage<Shared::Messages::Human::HumanSpawn>(Shared::Messages::ModMessages::MOD_HUMAN_SPAWN, [app](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanSpawn *msg) {
            auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            // Setup tracking info
            Create(e, msg->GetSpawnProfile());

            // Setup other components
            auto updateData = e.get_mut<Shared::Modules::HumanSync::UpdateData>();
            auto humanData  = e.get_mut<HumanData>();

            const auto carPassenger = msg->GetCarPassenger();
            if (carPassenger.carId) {
                updateData->carPassenger.carId      = carPassenger.carId;
                updateData->carPassenger.seatId     = carPassenger.seatId;
                humanData->carPassenger.enterState  = STATE_ENTERING;
                humanData->carPassenger.enterForced = true;
            }

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

            auto updateData = e.get_mut<Shared::Modules::HumanSync::UpdateData>();
            *updateData     = msg->GetData();

            Update(e);
        });
        net->RegisterMessage<Shared::Messages::Human::HumanSelfUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_SELF_UPDATE, [app](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanSelfUpdate *msg) {
            const auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            auto trackingData = e.get_mut<Core::Modules::Human::Tracking>();
            if (!trackingData) {
                return;
            }

            auto frame       = e.get_mut<Framework::World::Modules::Base::Frame>();
            frame->modelHash = msg->GetSpawnProfile();

            // update actor data
        });

        InitRPCs(app);
    }

    void Human::InitRPCs(Application *app) {
        const auto net = app->GetNetworkingEngine()->GetNetworkClient();

        net->RegisterGameRPC<Shared::RPC::HumanShoot>([app](SLNet::RakNetGUID guid, Shared::RPC::HumanShoot *msg) {
            const auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            auto trackingData = e.get_mut<Core::Modules::Human::Tracking>();
            if (!trackingData) {
                return;
            }

            const auto wepController = trackingData->human->GetHumanWeaponController();
            SDK::ue::sys::math::C_Vector dir, pos;
            pos = {msg->GetAimPos().x, msg->GetAimPos().y, msg->GetAimPos().z};
            dir = {msg->GetAimDir().x, msg->GetAimDir().y, msg->GetAimDir().z};
            wepController->DoShot(nullptr, &pos, &dir, msg->GetUnk0(), msg->GetUnk1());
        });

        net->RegisterGameRPC<Shared::RPC::HumanReload>([app](SLNet::RakNetGUID guid, Shared::RPC::HumanReload *msg) {
            const auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            auto trackingData = e.get_mut<Core::Modules::Human::Tracking>();
            if (!trackingData) {
                return;
            }

            const auto wepController = trackingData->human->GetHumanWeaponController();
            wepController->DoWeaponReloadInventory(msg->GetUnk0());
        });

        net->RegisterGameRPC<Shared::RPC::HumanChangeSkin>([app](SLNet::RakNetGUID guid, Shared::RPC::HumanChangeSkin *msg) {
            if (!msg->Valid())
                return;

            const auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            auto trackingData = e.get_mut<Core::Modules::Human::Tracking>();
            if (!trackingData) {
                return;
            }

            // todo change skin
        });
    }

    void Human::UpdateTransform(flecs::entity e) {
        const auto trackingData = e.get<Core::Modules::Human::Tracking>();
        if (!trackingData || !trackingData->human) {
            return;
        }

        // Update basic data
        const auto tr = e.get<Framework::World::Modules::Base::Transform>();
        auto interp   = e.get_mut<Interpolated>();
        if (interp) {
            // todo reset lerp
            const auto humanPos = trackingData->human->GetPos();
            const auto humanRot = trackingData->human->GetRot();
            interp->interpolator.GetPosition()->SetTargetValue({humanPos.x, humanPos.y, humanPos.z}, tr->pos, MafiaMP::Core::gApplication->GetTickInterval());
            interp->interpolator.GetRotation()->SetTargetValue({humanRot.w, humanRot.x, humanRot.y, humanRot.z}, tr->rot, MafiaMP::Core::gApplication->GetTickInterval());
        }
        else {
            SDK::ue::sys::math::C_Vector newPos    = {tr->pos.x, tr->pos.y, tr->pos.z};
            SDK::ue::sys::math::C_Quat newRot      = {tr->rot.x, tr->rot.y, tr->rot.z, tr->rot.w};
            SDK::ue::sys::math::C_Matrix transform = {};
            transform.Identity();
            transform.SetRot(newRot);
            transform.SetPos(newPos);
            trackingData->human->SetTransform(transform);
        }
    }
} // namespace MafiaMP::Core::Modules
