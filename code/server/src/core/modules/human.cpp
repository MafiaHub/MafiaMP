#include "human.h"

#include "world/modules/base.hpp"

#include "shared/messages/human/human_despawn.h"
#include "shared/messages/human/human_self_update.h"
#include "shared/messages/human/human_spawn.h"
#include "shared/messages/human/human_update.h"
#include "shared/modules/human_sync.hpp"
#include "vehicle.h"

#include "core/builtins/player.h"

#include "shared/game_rpc/human/human_shoot.h"
#include "shared/game_rpc/human/human_reload.h"
#include "shared/game_rpc/human/human_death.h"

#include <flecs/flecs.h>

namespace MafiaMP::Core::Modules {
    Human::Human(flecs::world &world) {
        world.module<Human>();
    }

    void Human::Create(Framework::Networking::NetworkServer *net, flecs::entity e) {
        auto frame       = e.get_mut<Framework::World::Modules::Base::Frame>();
        frame->modelHash = 335218123840277515; /* TODO */

        e.add<Shared::Modules::HumanSync::UpdateData>();

        auto er = e.get_mut<Framework::World::Modules::Base::Streamer>();
        er->collectRangeExemptEntitiesProc = [](flecs::entity e, Framework::World::Modules::Base::Streamer &streamer) {
            const auto updateData = e.get<Shared::Modules::HumanSync::UpdateData>();
            
            auto carEnt = Framework::CoreModules::GetWorldEngine()->WrapEntity(updateData->carPassenger.carId);
            if (carEnt.is_valid() && carEnt.is_alive()) {
                streamer.rangeExemptEntities.insert(carEnt.id());

                const auto carData = carEnt.get<Modules::Vehicle::CarData>();
                if (carData) {
                    for (int i = 0; i < 4 /* TODO: use MAX_SEATS constexpr var */; ++i) {
                        if (e.id() != carData->seats[i])
                            streamer.rangeExemptEntities.insert(carData->seats[i]);
                    }
                }
            }
        };

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
            // const auto frame            = e.get<Framework::World::Modules::Base::Frame>();

            Shared::Messages::Human::HumanUpdate humanUpdate {};
            humanUpdate.SetServerID(e.id());
            humanUpdate.SetData(*trackingMetadata);
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
            const auto newData = msg->GetData();
            const auto& carPassenger = newData.carPassenger;

            // TODO improve this code
            if (trackingMetadata->carPassenger.carId != carPassenger.carId) {
                if (trackingMetadata->carPassenger.carId) {
                    const auto carEnt = srv->WrapEntity(trackingMetadata->carPassenger.carId);

                    if (carEnt.is_alive()) {
                        auto car                                          = carEnt.get_mut<Modules::Vehicle::CarData>();
                        car->seats[trackingMetadata->carPassenger.seatId] = 0;
                    }
                }
                if (carPassenger.carId) {
                    const auto carEnt = srv->WrapEntity(carPassenger.carId);

                    if (carEnt.is_alive()) {
                        auto car                        = carEnt.get_mut<Modules::Vehicle::CarData>();
                        car->seats[carPassenger.seatId] = e.id();

                        // TODO rework so that we can ensure player doesn't sit in fully occupied vehicle etc
                    }
                }
            }

            *trackingMetadata = newData;
        });

        InitRPCs(srv, net);
    }

    void Human::InitRPCs(std::shared_ptr<Framework::World::ServerEngine> srv, Framework::Networking::NetworkServer *net) {
        net->RegisterGameRPC<Shared::RPC::HumanShoot>([srv, net](SLNet::RakNetGUID guid, Shared::RPC::HumanShoot *msg) {
            const auto e = srv->GetEntityByGUID(guid.g);
            if (!e.is_alive()) {
                return;
            }

            FW_SEND_SERVER_COMPONENT_GAME_RPC_EXCEPT(Shared::RPC::HumanShoot, e, guid, msg->GetAimPos(), msg->GetAimDir(), msg->GetUnk0(), msg->GetUnk1());
        });

        net->RegisterGameRPC<Shared::RPC::HumanReload>([srv, net](SLNet::RakNetGUID guid, Shared::RPC::HumanReload *msg) {
            const auto e = srv->GetEntityByGUID(guid.g);
            if (!e.is_alive()) {
                return;
            }

            FW_SEND_SERVER_COMPONENT_GAME_RPC_EXCEPT(Shared::RPC::HumanReload, e, guid, msg->GetUnk0());
        });

        net->RegisterGameRPC<Shared::RPC::HumanDeath>([srv, net](SLNet::RakNetGUID guid, Shared::RPC::HumanDeath *msg) {
            const auto e = srv->GetEntityByGUID(guid.g);
            if (!e.is_alive()) {
                return;
            }

            Scripting::Human::EventPlayerDied(e);
        });
    }
} // namespace MafiaMP::Core::Modules
