#include "human.h"

#include "utils/time.h"
#include "world/modules/base.hpp"

#include "shared/messages/human/human_despawn.h"
#include "shared/messages/human/human_self_update.h"
#include "shared/messages/human/human_spawn.h"
#include "shared/messages/human/human_update.h"
#include "shared/modules/human_sync.hpp"
#include "vehicle.h"

#include "core/builtins/player.h"

#include "shared/game_rpc/human/human_death.h"
#include "shared/game_rpc/human/human_reload.h"
#include "shared/game_rpc/human/human_shoot.h"

#include <flecs/flecs.h>

namespace MafiaMP::Core::Modules {
    Human::Human(flecs::world &world) {
        world.module<Human>();
    }

    void Human::Create(Framework::Networking::NetworkServer *net, flecs::entity e) {
        auto &frame     = e.ensure<Framework::World::Modules::Base::Frame>();
        frame.modelHash = 335218123840277515; /* TODO */

        e.add<Shared::Modules::HumanSync::UpdateData>();
        e.add<Framework::World::Modules::Base::RemovedOnResourceReload>();

        auto er                            = e.get_mut<Framework::World::Modules::Base::Streamer>();
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

        // Message sending is handled by observers set up in SetupMessages
    }

    void Human::SetupMessages(std::shared_ptr<Framework::World::ServerEngine> srv, Framework::Networking::NetworkServer *net) {
        auto world = srv->GetWorld();

        // Observer: Send HumanSpawn when StreamedTo relation is added to Human entities
        world->observer()
            .with<Framework::World::Modules::Base::StreamedTo>(flecs::Wildcard)
            .with<Shared::Modules::HumanSync::UpdateData>()
            .event(flecs::OnAdd)
            .each([net](flecs::iter& it, size_t row) {
                auto e = it.entity(row);
                auto streamerEntity = it.pair(0).second();
                if (!streamerEntity.is_valid() || !streamerEntity.is_alive())
                    return;

                auto streamer = streamerEntity.get<Framework::World::Modules::Base::Streamer>();
                if (!streamer)
                    return;

                const auto frame = e.get<Framework::World::Modules::Base::Frame>();
                Shared::Messages::Human::HumanSpawn humanSpawn;
                humanSpawn.FromParameters(frame ? frame->modelHash : 0, streamer->nickname, streamer->playerIndex);
                humanSpawn.SetServerID(e.id());

                const auto trackingMetadata = e.get<Shared::Modules::HumanSync::UpdateData>();
                if (trackingMetadata) {
                    humanSpawn.SetCarPassenger(trackingMetadata->carPassenger.carId, trackingMetadata->carPassenger.seatId);
                }

                net->Send(humanSpawn, streamer->guid);
            });

        // Observer: Send HumanDespawn when StreamedTo relation is removed from Human entities
        world->observer()
            .with<Framework::World::Modules::Base::StreamedTo>(flecs::Wildcard)
            .with<Shared::Modules::HumanSync::UpdateData>()
            .event(flecs::OnRemove)
            .each([net](flecs::iter& it, size_t row) {
                auto e = it.entity(row);
                auto streamerEntity = it.pair(0).second();
                if (!streamerEntity.is_valid() || !streamerEntity.is_alive())
                    return;

                auto streamer = streamerEntity.get<Framework::World::Modules::Base::Streamer>();
                if (!streamer)
                    return;

                Shared::Messages::Human::HumanDespawn humanDespawn;
                humanDespawn.SetServerID(e.id());
                net->Send(humanDespawn, streamer->guid);
            });

        // System: Send periodic HumanUpdate/HumanSelfUpdate for streamed Human entities
        world->system<Shared::Modules::HumanSync::UpdateData, Framework::World::Modules::Base::Streamable>("HumanStreamUpdates")
            .kind(flecs::PostUpdate)
            .each([net](flecs::entity e, Shared::Modules::HumanSync::UpdateData& updateData, Framework::World::Modules::Base::Streamable& streamable) {
                // Skip entities not being streamed or marked as no-update
                if (e.has<Framework::World::Modules::Base::NoTickUpdates>())
                    return;

                // Iterate over all streamers this entity is streamed to
                e.each<Framework::World::Modules::Base::StreamedTo>([&](flecs::entity streamerEntity) {
                    if (!streamerEntity.is_valid() || !streamerEntity.is_alive())
                        return;

                    auto streamer = streamerEntity.get<Framework::World::Modules::Base::Streamer>();
                    if (!streamer)
                        return;

                    // Check timing via the StreamedTo relation data
                    auto streamData = e.get_mut<Framework::World::Modules::Base::StreamedTo>(streamerEntity);
                    if (!streamData)
                        return;

                    double currentTime = static_cast<double>(Framework::Utils::Time::GetTime());
                    if (currentTime - streamData->lastUpdate < streamable.updateInterval)
                        return;

                    streamData->lastUpdate = currentTime;

                    // Send appropriate message based on ownership
                    if (streamer->guid != streamable.owner) {
                        // Non-owner gets regular update
                        Shared::Messages::Human::HumanUpdate humanUpdate {};
                        humanUpdate.SetServerID(e.id());
                        humanUpdate.SetData(updateData);
                        net->Send(humanUpdate, streamer->guid);
                    } else {
                        // Owner gets self update
                        Shared::Messages::Human::HumanSelfUpdate humanSelfUpdate;
                        humanSelfUpdate.SetServerID(e.id());
                        net->Send(humanSelfUpdate, streamer->guid);
                    }
                });
            });

        net->RegisterMessage<Shared::Messages::Human::HumanUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_UPDATE, [srv](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanUpdate *msg) {
            const auto e = srv->WrapEntity(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }
            if (!srv->IsEntityOwner(e, guid.g)) {
                return;
            }

            auto trackingMetadata    = e.get_mut<Shared::Modules::HumanSync::UpdateData>();
            const auto newData       = msg->GetData();
            const auto &carPassenger = newData.carPassenger;

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

            Scripting::Player::EventPlayerDied(e);
        });
    }
} // namespace MafiaMP::Core::Modules
