#include "vehicle.h"

#include "networking/network_peer.h"
#include "world/modules/base.hpp"

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

        e.add<CarData>();
        e.add<Framework::World::Modules::Base::RemovedOnGameModeReload>();

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

        es->modEvents.spawnProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
            const auto frame      = e.get<Framework::World::Modules::Base::Frame>();
            auto trackingMetadata = e.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            Shared::Messages::Vehicle::VehicleSpawn vehicleSpawn;
            vehicleSpawn.FromParameters(frame->modelName.c_str(), *trackingMetadata);
            vehicleSpawn.SetServerID(e.id());
            net->Send(vehicleSpawn, guid);
            // todo other stuff
            return true;
        };

        es->modEvents.despawnProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
            Shared::Messages::Vehicle::VehicleDespawn vehicleDespawn;
            vehicleDespawn.SetServerID(e.id());
            net->Send(vehicleDespawn, guid);
            return true;
        };

        es->modEvents.updateProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
            auto trackingMetadata = e.get_mut<Shared::Modules::VehicleSync::UpdateData>();

            Shared::Messages::Vehicle::VehicleUpdate vehicleUpdate {};
            vehicleUpdate.SetServerID(e.id());
            vehicleUpdate.SetData(*trackingMetadata);
            net->Send(vehicleUpdate, guid);
            return true;
        };

        // TODO: deprecate in favor of RPCs
        /*es->modEvents.ownerUpdateProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
            auto trackingMetadata = e.get_mut<Shared::Modules::VehicleSync::UpdateData>();

            Shared::Messages::Vehicle::VehicleOwnerUpdate vehicleUpdate {};
            vehicleUpdate.SetServerID(e.id());
            vehicleUpdate.SetData(*trackingMetadata);
            net->Send(vehicleUpdate, guid);
            return true;
        };*/

        return e;
    }

    void Vehicle::SetupMessages(std::shared_ptr<Framework::World::ServerEngine> srv, Framework::Networking::NetworkServer *net) {
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
