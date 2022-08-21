#pragma once

#include <flecs/flecs.h>

#include "../sdk/c_game.h"
#include "../sdk/entities/c_vehicle.h"
#include "../game/streaming/entity_factory.h"
#include "../sdk/entities/c_car.h"
#include "../sdk/mafia/framework/c_mafia_framework.h"
#include "../sdk/mafia/framework/c_mafia_framework_interfaces.h"

#include "../game/streaming/entity_tracking_info.h"

#include "../shared/modules/vehicle_sync.hpp"

#include <world/modules/base.hpp>
#include <utils/interpolator.h>

#include "../shared/modules/vehicle_sync.hpp"
#include "../shared/messages/vehicle/vehicle_despawn.h"
#include "../shared/messages/vehicle/vehicle_spawn.h"
#include "../shared/messages/vehicle/vehicle_update.h"

namespace MafiaMP::Core::Modules {
    struct Vehicle {
        struct Tracking {
            SDK::C_Car *car                                 = nullptr;
            Game::Streaming::EntityTrackingInfo *info            = nullptr;
        };

        struct Interpolated {
            Framework::Utils::Interpolator interpolator = {};
        };

        Vehicle(flecs::world &world) {
            world.module<Vehicle>();

            world.component<Tracking>();
            world.component<Interpolated>();

            world.system<Tracking, Shared::Modules::VehicleSync::UpdateData, Framework::World::Modules::Base::Transform>("UpdateOwnedVehicles").each([](flecs::entity e, Tracking &tracking, Shared::Modules::VehicleSync::UpdateData &metadata, Framework::World::Modules::Base::Transform &tr) {
                const auto myGUID = Core::gApplication->GetNetworkingEngine()->GetNetworkClient()->GetPeer()->GetMyGUID();
                if (tracking.car && Framework::World::Modules::Base::IsEntityOwnedBy(e, myGUID.g) == true) {
                    const auto car = tracking.car;
                    SDK::ue::sys::math::C_Vector carPos = ((SDK::C_Actor*)car)->GetPos();
                    SDK::ue::sys::math::C_Quat carRot = ((SDK::C_Actor*)car)->GetRot();
                    SDK::C_Vehicle* vehicle = car->GetVehicle();
                    SDK::ue::sys::math::C_Vector vehicleVelocity = vehicle->GetSpeed();
                    SDK::ue::sys::math::C_Vector vehicleAngularVelocity = vehicle->GetAngularSpeed();
                    tr.pos = {carPos.x, carPos.y, carPos.z};
                    tr.rot = {carRot.w, carRot.x, carRot.y, carRot.z};

                    metadata.gear = car->GetGear();
                    metadata.horn = vehicle->GetHorn();
                    metadata.power = vehicle->GetPower();
                    metadata.brake = vehicle->GetBrake();
                    metadata.handbrake = vehicle->GetHandbrake();
                    metadata.steer = vehicle->GetSteer();
                    metadata.velocity = { vehicleVelocity.x, vehicleVelocity.y, vehicleVelocity.z };
                    metadata.angularVelocity = { vehicleAngularVelocity.x, vehicleAngularVelocity.y, vehicleAngularVelocity.z };
                }
            });

            world.system<Tracking, Interpolated>("UpdateRemoteVehicles").each([](flecs::entity e, Tracking &tracking, Interpolated &interpolated) {
                const auto myGUID = Core::gApplication->GetNetworkingEngine()->GetNetworkClient()->GetPeer()->GetMyGUID();
                if (tracking.car && Framework::World::Modules::Base::IsEntityOwnedBy(e, myGUID.g) == false) {
                    const auto car = tracking.car;
                    const auto vehiclePos = car->GetPos();
                    const auto vehicleRot = car->GetRot();
                    const auto newPos = interpolated.interpolator.GetPosition()->UpdateTargetValue({vehiclePos.x, vehiclePos.y, vehiclePos.z});
                    const auto newRot = interpolated.interpolator.GetRotation()->UpdateTargetValue({vehicleRot.w, vehicleRot.x, vehicleRot.y, vehicleRot.z});
                    SDK::ue::sys::math::C_Matrix transform = {};
                    transform.Identity();
                    transform.SetRot({newRot.x, newRot.y, newRot.z, newRot.w});
                    transform.SetPos({newPos.x, newPos.y, newPos.z});
                    car->GetVehicle()->SetVehicleMatrix(transform, SDK::ue::sys::core::E_TransformChangeType::DEFAULT);
                }
            });
        }

        static inline void Create(flecs::entity e, std::string modelName) {
            auto info = Core::gApplication->GetEntityFactory()->RequestVehicle(modelName);
            auto trackingData = e.get_mut<Core::Modules::Vehicle::Tracking>();
            trackingData->info = info;
            trackingData->car = nullptr;

            auto interp = e.get_mut<Interpolated>();
            interp->interpolator.GetPosition()->SetCompensationFactor(1.5f);

            const auto OnVehicleRequestFinish = [](Game::Streaming::EntityTrackingInfo *info, bool success) {
                if (success) {
                    auto car = reinterpret_cast<SDK::C_Car *>(info->GetEntity());
                    if (!car) {
                        return;
                    }
                    car->GameInit();
                    car->Activate();
                    car->Unlock();

                    const auto ent = info->GetNetworkEntity();
                    const auto tr = ent.get<Framework::World::Modules::Base::Transform>();
                    SDK::ue::sys::math::C_Vector newPos = { tr->pos.x, tr->pos.y, tr->pos.z };
                    SDK::ue::sys::math::C_Quat newRot   = { tr->rot.x, tr->rot.y, tr->rot.z, tr->rot.w };
                    SDK::ue::sys::math::C_Matrix transform = {};
                    transform.Identity();
                    transform.SetRot(newRot);
                    transform.SetPos(newPos);
                    car->GetVehicle()->SetVehicleMatrix(transform, SDK::ue::sys::core::E_TransformChangeType::DEFAULT);

                    auto trackingData = ent.get_mut<Core::Modules::Vehicle::Tracking>();
                    trackingData->car = car;

                    auto streamable = ent.get_mut<Framework::World::Modules::Base::Streamable>();
                    streamable->modEvents.updateProc = [](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                        const auto updateData = e.get<Shared::Modules::VehicleSync::UpdateData>();
                        const auto serverID = e.get<Framework::World::Modules::Base::ServerID>();

                        Shared::Messages::Vehicle::VehicleUpdate vehicleUpdate{};
                        vehicleUpdate.SetServerID(serverID->id);
                        vehicleUpdate.SetGear(updateData->gear);
                        vehicleUpdate.SetHorn(updateData->horn);
                        vehicleUpdate.SetPower(updateData->power);
                        vehicleUpdate.SetBrake(updateData->brake);
                        vehicleUpdate.SetHandbrake(updateData->handbrake);
                        vehicleUpdate.SetSteer(updateData->steer);
                        vehicleUpdate.SetVelocity(updateData->velocity);
                        vehicleUpdate.SetAngularVelocity(updateData->angularVelocity);
                        peer->Send(vehicleUpdate, guid);
                        return true;
                    };
                }
            };

            const auto OnVehicleReturned = [](Game::Streaming::EntityTrackingInfo *info, bool wasCreated) {
                if (!info) {
                    return;
                }
                auto car = reinterpret_cast<SDK::C_Car *>(info->GetEntity());
                if (wasCreated && car) {
                    car->Deactivate();
                    car->GameDone();
                    car->Release();
                }
            };

            // setup tracking callbacks
            info->SetRequestFinishCallback(OnVehicleRequestFinish);
            info->SetReturnCallback(OnVehicleReturned);
            info->SetNetworkEntity(e);
        }

        static inline void Update(flecs::entity e) {
            const auto trackingData = e.get<Core::Modules::Vehicle::Tracking>();
            if (!trackingData || !trackingData->car) {
                return;
            }

            // Update basic data
            const auto tr = e.get<Framework::World::Modules::Base::Transform>();
            auto interp = e.get_mut<Interpolated>();
            if (interp) {
                const auto vehiclePos = trackingData->car->GetPos();
                const auto vehicleRot = trackingData->car->GetRot();
                interp->interpolator.GetPosition()->SetTargetValue({ vehiclePos.x, vehiclePos.y, vehiclePos.z }, tr->pos, MafiaMP::Core::gApplication->GetTickInterval());
                interp->interpolator.GetRotation()->SetTargetValue({vehicleRot.w,vehicleRot.x, vehicleRot.y, vehicleRot.z}, tr->rot, MafiaMP::Core::gApplication->GetTickInterval());
            } else {
                SDK::ue::sys::math::C_Vector newPos = { tr->pos.x, tr->pos.y, tr->pos.z };
                SDK::ue::sys::math::C_Quat newRot   = { tr->rot.x, tr->rot.y, tr->rot.z, tr->rot.w };
                SDK::ue::sys::math::C_Matrix transform = {};
                trackingData->car->SetPos(newPos);
                trackingData->car->SetRot(newRot);
            }

            auto updateData = e.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            SDK::C_Vehicle* vehicle = trackingData->car->GetVehicle();
            vehicle->SetGear(updateData->gear);
            vehicle->SetHorn(updateData->horn);
            vehicle->SetPower(updateData->power);
            vehicle->SetBrake(updateData->brake, false);
            vehicle->SetHandbrake(updateData->handbrake, false);
            vehicle->SetSteer(updateData->steer);
            vehicle->SetSpeed({ updateData->velocity.x, updateData->velocity.y, updateData->velocity.z }, false, false);
            vehicle->SetAngularSpeed({ updateData->angularVelocity.x, updateData->angularVelocity.y, updateData->angularVelocity.z }, false);
        }

        static inline void Remove(flecs::entity e) {
            auto trackingData = e.get_mut<Core::Modules::Vehicle::Tracking>();
            if (!trackingData) {
                return;
            }

            Core::gApplication->GetEntityFactory()->ReturnEntity(trackingData->info);
            trackingData->info = nullptr;
        }

        static inline void SetupMessages(Application *app) {
            const auto net = app->GetNetworkingEngine()->GetNetworkClient();
            net->RegisterMessage<Shared::Messages::Vehicle::VehicleSpawn>(Shared::Messages::ModMessages::MOD_VEHICLE_SPAWN, [app](SLNet::RakNetGUID guid, Shared::Messages::Vehicle::VehicleSpawn *msg) {
                auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
                if (!e.is_alive()) {
                    return;
                }

                // Setup tracking info
                Create(e, msg->GetModelName());

                // Setup other components
                e.add<Shared::Modules::VehicleSync::UpdateData>();
            });
            net->RegisterMessage<Shared::Messages::Vehicle::VehicleDespawn>(Shared::Messages::ModMessages::MOD_VEHICLE_DESPAWN, [app](SLNet::RakNetGUID guid, Shared::Messages::Vehicle::VehicleDespawn *msg) {
                const auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
                if (!e.is_alive()) {
                    return;
                }

                Remove(e);
            });
            net->RegisterMessage<Shared::Messages::Vehicle::VehicleUpdate>(Shared::Messages::ModMessages::MOD_VEHICLE_UPDATE, [app](SLNet::RakNetGUID guid, Shared::Messages::Vehicle::VehicleUpdate *msg) {
                const auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
                if (!e.is_alive()) {
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
                Update(e);
            });
        }
    };
} // namespace MafiaMP::Core::Modules
