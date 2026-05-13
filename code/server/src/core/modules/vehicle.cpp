#include "vehicle.h"

#include "networking/network_peer.h"
#include "utils/time.h"
#include "world/modules/base.hpp"

#include "shared/game_rpc/vehicle/vehicle_player_enter.h"
#include "shared/game_rpc/vehicle/vehicle_player_leave.h"

#include "shared/messages/vehicle/vehicle_despawn.h"
#include "shared/messages/vehicle/vehicle_owner_update.h"
#include "shared/messages/vehicle/vehicle_spawn.h"
#include "shared/messages/vehicle/vehicle_update.h"
#include "shared/modules/vehicle_sync.hpp"

#include "core/builtins/vehicle.h"

#include <utils/safe_string.h>

#include <flecs/flecs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace MafiaMP::Core::Modules {
    Vehicle::Vehicle(flecs::world &world) {
        world.module<Vehicle>();

        world.component<CarData>();

        world.system<CarData>("FixVehicleSeats").each([](flecs::entity e, CarData &updateData) {
            for (auto &seat : updateData.seats) {
                if (seat) {
                    const auto playerEnt = flecs::entity(e.world(), seat);
                    if (!playerEnt.is_alive()) {
                        seat = 0;
                    }
                }
            }
        });
    }

    flecs::entity Vehicle::Create(MafiaMP::Server *server) {
        const auto net = server->GetNetworkingEngine()->GetNetworkServer();
        auto e         = server->GetWorldEngine()->CreateEntity();
        server->GetStreamingFactory()->SetupServer(e, SLNet::UNASSIGNED_RAKNET_GUID.g);
        auto &frame     = e.ensure<Framework::World::Modules::Base::Frame>();
        frame.modelName = "berkley_810"; /* TODO */

        e.add<Shared::Modules::VehicleSync::UpdateData>();
        e.add<CarData>();
        e.add<Framework::World::Modules::Base::RemovedOnResourceReload>();

        auto es = e.get_mut<Framework::World::Modules::Base::Streamable>();

        es->assignOwnerProc = [](flecs::entity e, Framework::World::Modules::Base::Streamable &es) {
            const auto carData = e.get<CarData>();
            for (const auto seat : carData->seats) {
                if (seat) {
                    const auto playerEnt = flecs::entity(e.world(), seat);
                    if (!playerEnt.is_alive())
                        continue;

                    const auto streamable = playerEnt.get<Framework::World::Modules::Base::Streamable>();
                    es.owner              = streamable->owner;
                    return true;
                }
            }
            return false;
        };

        // Message sending is handled by observers set up in SetupMessages

        return e;
    }

    void Vehicle::SetupMessages(std::shared_ptr<Framework::World::ServerEngine> srv, Framework::Networking::NetworkServer *net) {
        auto world = srv->GetWorld();

        // Observer: Send VehicleSpawn when StreamedTo relation is added to Vehicle entities
        world->observer()
            .with<Framework::World::Modules::Base::StreamedTo>(flecs::Wildcard)
            .with<Shared::Modules::VehicleSync::UpdateData>()
            .event(flecs::OnAdd)
            .each([net](flecs::iter& it, size_t row) {
                auto e = it.entity(row);
                auto streamerEntity = it.pair(0).second();
                if (!streamerEntity.is_valid() || !streamerEntity.is_alive())
                    return;

                auto streamer = streamerEntity.get<Framework::World::Modules::Base::Streamer>();
                if (!streamer)
                    return;

                const auto frame      = e.get<Framework::World::Modules::Base::Frame>();
                auto trackingMetadata = e.get<Shared::Modules::VehicleSync::UpdateData>();
                if (!frame || !trackingMetadata)
                    return;

                Shared::Messages::Vehicle::VehicleSpawn vehicleSpawn;
                vehicleSpawn.FromParameters(frame->modelName.c_str(), *trackingMetadata);
                vehicleSpawn.SetServerID(e.id());
                net->Send(vehicleSpawn, streamer->guid);
            });

        // Observer: Send VehicleDespawn when StreamedTo relation is removed from Vehicle entities
        world->observer()
            .with<Framework::World::Modules::Base::StreamedTo>(flecs::Wildcard)
            .with<Shared::Modules::VehicleSync::UpdateData>()
            .event(flecs::OnRemove)
            .each([net](flecs::iter& it, size_t row) {
                auto e = it.entity(row);
                auto streamerEntity = it.pair(0).second();
                if (!streamerEntity.is_valid() || !streamerEntity.is_alive())
                    return;

                auto streamer = streamerEntity.get<Framework::World::Modules::Base::Streamer>();
                if (!streamer)
                    return;

                Shared::Messages::Vehicle::VehicleDespawn vehicleDespawn;
                vehicleDespawn.SetServerID(e.id());
                net->Send(vehicleDespawn, streamer->guid);
            });

        // System: Send periodic VehicleUpdate for streamed Vehicle entities
        world->system<Shared::Modules::VehicleSync::UpdateData, Framework::World::Modules::Base::Streamable>("VehicleStreamUpdates")
            .kind(flecs::PostUpdate)
            .each([net](flecs::entity e, Shared::Modules::VehicleSync::UpdateData& updateData, Framework::World::Modules::Base::Streamable& streamable) {
                // Skip entities marked as no-update
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

                    // Send update (vehicles don't distinguish owner/non-owner updates currently)
                    Shared::Messages::Vehicle::VehicleUpdate vehicleUpdate {};
                    vehicleUpdate.SetServerID(e.id());
                    vehicleUpdate.SetData(updateData);
                    net->Send(vehicleUpdate, streamer->guid);
                });
            });

        net->RegisterMessage<Shared::Messages::Vehicle::VehicleUpdate>(Shared::Messages::ModMessages::MOD_VEHICLE_UPDATE, [srv](SLNet::RakNetGUID guid, Shared::Messages::Vehicle::VehicleUpdate *msg) {
            const auto e = srv->WrapEntity(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }
            if (!srv->IsEntityOwner(e, guid.g)) {
                return;
            }

            auto updateData = e.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            *updateData     = msg->GetData();
        });

        InitRPCs(srv, net);
    }

    void Vehicle::InitRPCs(std::shared_ptr<Framework::World::ServerEngine> srv, Framework::Networking::NetworkServer *net) {
        net->RegisterGameRPC<Shared::RPC::VehiclePlayerEnter>([srv](SLNet::RakNetGUID guid, Shared::RPC::VehiclePlayerEnter *msg) {
            const auto playerEntity = srv->GetEntityByGUID(guid.g);
            if (!playerEntity.is_alive()) {
                return;
            }

            const auto vehicleEntity = srv->WrapEntity(msg->vehicleId);
            if (!vehicleEntity.is_valid()) {
                return;
            }

            Scripting::Vehicle::EventVehiclePlayerEnter(vehicleEntity, playerEntity, msg->seatIndex);
        });

        net->RegisterGameRPC<Shared::RPC::VehiclePlayerLeave>([srv](SLNet::RakNetGUID guid, Shared::RPC::VehiclePlayerLeave *msg) {
            const auto playerEntity = srv->GetEntityByGUID(guid.g);
            if (!playerEntity.is_alive()) {
                return;
            }

            const auto vehicleEntity = srv->WrapEntity(msg->vehicleId);
            if (!vehicleEntity.is_valid()) {
                return;
            }

            Scripting::Vehicle::EventVehiclePlayerLeave(vehicleEntity, playerEntity);
        });
    }
} // namespace MafiaMP::Core::Modules
