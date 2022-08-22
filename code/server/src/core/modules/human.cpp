#pragma once

#include "human.h"

#include "world/modules/base.hpp"

#include "shared/messages/human/human_despawn.h"
#include "shared/messages/human/human_self_update.h"
#include "shared/messages/human/human_spawn.h"
#include "shared/messages/human/human_update.h"
#include "shared/modules/human_sync.hpp"
#include "shared/modules/vehicle_sync.hpp"

#include <flecs/flecs.h>

namespace MafiaMP::Core::Modules {
    Human::Human(flecs::world &world) {
        world.module<Human>();
    }

    void Human::Create(Framework::Networking::NetworkServer *net, flecs::entity e) {
        auto frame       = e.get_mut<Framework::World::Modules::Base::Frame>();
        frame->modelHash = 335218123840277515; /* TODO */

        e.add<Shared::Modules::HumanSync::UpdateData>();

        auto es = e.get_mut<Framework::World::Modules::Base::Streamable>();

        es->modEvents.spawnProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
            const auto frame = e.get<Framework::World::Modules::Base::Frame>();
            Shared::Messages::Human::HumanSpawn humanSpawn;
            humanSpawn.FromParameters(frame->modelHash);
            humanSpawn.SetServerID(e.id());

            const auto trackingMetadata = e.get<Shared::Modules::HumanSync::UpdateData>();
            humanSpawn.SetCarPassenger(trackingMetadata->carPassenger.carId, trackingMetadata->carPassenger.seatId);

            net->Send(humanSpawn, guid);
            // todo other stuff
            return true;
        };

        es->modEvents.despawnProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
            Shared::Messages::Human::HumanDespawn humanDespawn;
            humanDespawn.SetServerID(e.id());
            net->Send(humanDespawn, guid);
            return true;
        };

        es->modEvents.selfUpdateProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
            Shared::Messages::Human::HumanSelfUpdate humanSelfUpdate;
            humanSelfUpdate.SetServerID(e.id());
            net->Send(humanSelfUpdate, guid);
            return true;
        };

        es->modEvents.updateProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
            const auto trackingMetadata = e.get<Shared::Modules::HumanSync::UpdateData>();
            const auto frame            = e.get<Framework::World::Modules::Base::Frame>();

            Shared::Messages::Human::HumanUpdate humanUpdate {};
            humanUpdate.SetServerID(e.id());
            humanUpdate.SetCharStateHandlerType(trackingMetadata->_charStateHandlerType);
            humanUpdate.SetHealthPercent(trackingMetadata->_healthPercent);
            humanUpdate.SetMoveMode(trackingMetadata->_moveMode);
            humanUpdate.SetSprinting(trackingMetadata->_isSprinting);
            humanUpdate.SetSprintSpeed(trackingMetadata->_sprintSpeed);
            humanUpdate.SetStalking(trackingMetadata->_isStalking);
            humanUpdate.SetCarPassenger(trackingMetadata->carPassenger.carId, trackingMetadata->carPassenger.seatId);
            humanUpdate.SetSpawnProfile(frame->modelHash);
            
            const auto wepData = trackingMetadata->weaponData;
            humanUpdate.SetWeaponData({wepData.isAiming, wepData.isFiring});

            net->Send(humanUpdate, guid);
            return true;
        };
    }

    void Human::SetupMessages(std::shared_ptr<Framework::World::ServerEngine> srv, Framework::Networking::NetworkServer *net) {
        net->RegisterMessage<Shared::Messages::Human::HumanUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_UPDATE, [srv](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanUpdate *msg) {
            const auto e = srv->WrapEntity(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }
            if (!srv->IsEntityOwner(e, guid.g)) {
                return;
            }

            auto trackingMetadata                   = e.get_mut<Shared::Modules::HumanSync::UpdateData>();
            trackingMetadata->_charStateHandlerType = msg->GetCharStateHandlerType();
            trackingMetadata->_healthPercent        = msg->GetHealthPercent();
            trackingMetadata->_isSprinting          = msg->IsSprinting();
            trackingMetadata->_isStalking           = msg->IsStalking();
            trackingMetadata->_moveMode             = msg->GetMoveMode();
            trackingMetadata->_sprintSpeed          = msg->GetSprintSpeed();

            const auto wepData = msg->GetWeaponData();
            trackingMetadata->weaponData = {wepData.isAiming, wepData.isFiring};

            const auto carPassenger = msg->GetCarPassenger();

            // TODO improve this code
            if (trackingMetadata->carPassenger.carId != carPassenger.carId) {
                if (trackingMetadata->carPassenger.carId) {
                    const auto carEnt = srv->WrapEntity(trackingMetadata->carPassenger.carId);

                    if (carEnt.is_alive()) {
                        auto car                                          = carEnt.get_mut<Shared::Modules::VehicleSync::UpdateData>();
                        car->seats[trackingMetadata->carPassenger.seatId] = 0;
                    }
                }
                if (carPassenger.carId) {
                    const auto carEnt = srv->WrapEntity(carPassenger.carId);

                    if (carEnt.is_alive()) {
                        auto car                        = carEnt.get_mut<Shared::Modules::VehicleSync::UpdateData>();
                        car->seats[carPassenger.seatId] = e.id();

                        // TODO rework so that we can ensure player doesn't sit in fully occupied vehicle etc
                    }
                }
            }

            trackingMetadata->carPassenger.carId  = carPassenger.carId;
            trackingMetadata->carPassenger.seatId = carPassenger.seatId;
        });
    }
} // namespace MafiaMP::Core::Modules
