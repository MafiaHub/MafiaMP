#pragma once

#include <integrations/server/instance.h>

#include "networking/network_peer.h"
#include "world/modules/base.hpp"

#include "shared/messages/vehicle/vehicle_spawn.h"
#include "shared/messages/vehicle/vehicle_despawn.h"
#include "shared/messages/vehicle/vehicle_update.h"
#include "shared/modules/vehicle_sync.hpp"

#include <flecs/flecs.h>

namespace MafiaMP::Core::Modules {
    class Vehicle {
      public:

        Vehicle(flecs::world &world) {
            world.module<Vehicle>();
        }

        static inline void Create(Framework::Networking::NetworkServer *net, flecs::entity e) {
            auto frame = e.get_mut<Framework::World::Modules::Base::Frame>();
            frame->modelName = "berkley_810"; /* TODO */

            e.add<Shared::Modules::VehicleSync::UpdateData>();

            auto es = e.get_mut<Framework::World::Modules::Base::Streamable>();

            es->modEvents.spawnProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                const auto frame = e.get<Framework::World::Modules::Base::Frame>();
                Shared::Messages::Vehicle::VehicleSpawn vehicleSpawn;
                vehicleSpawn.FromParameters(frame->modelName.c_str());
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

        static inline void SetupMessages(std::shared_ptr<Framework::World::ServerEngine> srv, Framework::Networking::NetworkServer *net) {
            net->RegisterMessage<Shared::Messages::Vehicle::VehicleUpdate>(Shared::Messages::ModMessages::MOD_VEHICLE_UPDATE, [srv](SLNet::RakNetGUID guid, Shared::Messages::Vehicle::VehicleUpdate *msg) {
                const auto e = srv->WrapEntity(msg->GetServerID());
                if (!e.is_alive()) {
                    return;
                }
                if (!srv->IsEntityOwner(e, guid.g)) {
                    return;
                }

                auto updateData = e.get_mut<Shared::Modules::VehicleSync::UpdateData>();
                updateData->velocity = msg->GetVelocity();
                updateData->angularVelocity = msg->GetAngularVelocity();
                updateData->gear = msg->GetGear();
                updateData->horn = msg->GetHorn();
                updateData->power = msg->GetPower();
                updateData->brake = msg->GetBrake();
                updateData->handbrake = msg->GetHandbrake();
                updateData->steer = msg->GetSteer();
            });
        }
    };
} // namespace MafiaMP
