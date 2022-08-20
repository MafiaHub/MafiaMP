#pragma once

#include "networking/network_peer.h"
#include "world/modules/base.hpp"

#include "shared/messages/vehicle/vehicle_spawn.h"
#include "shared/messages/vehicle/vehicle_despawn.h"
#include "shared/messages/vehicle/vehicle_update.h"
#include "shared/modules/vehicle_sync.hpp"

#include <flecs/flecs.h>

namespace MafiaMP {
    class VehicleFactory {
      private:
        inline void SetupDefaults(flecs::entity e) {
            auto frame = e.get_mut<Framework::World::Modules::Base::Frame>();
            frame->modelName = "berkley_810"; /* TODO */

            e.add<Shared::Modules::VehicleSync::UpdateData>();
        }

      public:
        inline void SetupServer(Framework::Networking::NetworkServer *net, flecs::entity e) {
            SetupDefaults(e);

            auto es = e.get_mut<Framework::World::Modules::Base::Streamable>();

            es->modEvents.spawnProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                const auto frame = e.get<Framework::World::Modules::Base::Frame>();
                Shared::Messages::Vehicle::VehicleSpawn vehicleSpawn;
                vehicleSpawn.FromParameters(frame->modelName.c_str());
                vehicleSpawn.SetServerID(e.id());
                net->Send(vehicleSpawn, guid);
                printf("Vehicle spawned: %s\n", frame->modelName.c_str());
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
                const auto trackingMetadata = e.get<Shared::Modules::VehicleSync::UpdateData>();

                Shared::Messages::Vehicle::VehicleUpdate vehicleUpdate{};
                vehicleUpdate.SetServerID(e.id());
                vehicleUpdate.SetVelocity(trackingMetadata->velocity);
                vehicleUpdate.SetAngularVelocity(trackingMetadata->angularVelocity);
                vehicleUpdate.SetGear(trackingMetadata->gear);
                vehicleUpdate.SetHorn(trackingMetadata->horn);
                vehicleUpdate.SetPower(trackingMetadata->power);
                vehicleUpdate.SetBrake(trackingMetadata->brake);
                vehicleUpdate.SetHandbrake(trackingMetadata->handbrake);
                vehicleUpdate.SetSteer(trackingMetadata->steer);
                net->Send(vehicleUpdate, guid);
                return true;
            };
        }
    };
} // namespace MafiaMP
