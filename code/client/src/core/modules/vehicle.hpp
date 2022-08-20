#pragma once

#include <flecs/flecs.h>

#include "../sdk/c_game.h"
#include "../sdk/entities/c_car.h"
#include "../sdk/entities/c_vehicle.h"

#include "../game/streaming/entity_tracking_info.h"

#include "../shared/modules/vehicle_sync.hpp"

#include <world/modules/base.hpp>
#include <utils/interpolator.h>

namespace MafiaMP::Core::Modules {
    struct Vehicle {
        struct Tracking {
            SDK::C_Car *vehicle                                 = nullptr;
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
            });

            world.system<Tracking, Interpolated>("UpdateRemoteVehicle").each([](flecs::entity e, Tracking &tracking, Interpolated &interpolated) {
                /* if (tracking.vehicle && e.get<LocalPlayer>() == nullptr) {
                    const auto vehiclePos = tracking.vehicle->GetPos();
                    const auto vehicleRot = tracking.vehicle->GetRot();
                    const auto newPos = interpolated.interpolator.GetPosition()->UpdateTargetValue({vehiclePos.x, vehiclePos.y, vehiclePos.z});
                    const auto newRot = interpolated.interpolator.GetRotation()->UpdateTargetValue({vehicleRot.w, vehicleRot.x, vehicleRot.y, vehicleRot.z});
                    tracking.vehicle->SetPos({newPos.x, newPos.y, newPos.z});
                    tracking.vehicle->SetRot({newRot.x, newRot.y, newRot.z, newRot.w});
                } */
            });
        }

        static inline void CreateVehicle(flecs::entity e, std::string modelName) {
            auto info = Core::gApplication->GetEntityFactory()->RequestVehicle(modelName);
            auto trackingData = e.get_mut<Core::Modules::Vehicle::Tracking>();
            trackingData->info = info;
            trackingData->vehicle = nullptr;

            auto interp = e.get_mut<Interpolated>();
            interp->interpolator.GetPosition()->SetCompensationFactor(1.5f);

            const auto OnVehicleRequestFinish = [trackingData](Game::Streaming::EntityTrackingInfo *info, bool success) {
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

                    trackingData->vehicle = car;
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

        static inline void UpdateVehicle(flecs::entity e) {
            const auto trackingData = e.get<Core::Modules::Vehicle::Tracking>();
            if (!trackingData || !trackingData->vehicle) {
                return;
            }

            // Update basic data
            const auto tr = e.get<Framework::World::Modules::Base::Transform>();
            auto interp = e.get_mut<Interpolated>();
            if (interp) {
                const auto vehiclePos = trackingData->vehicle->GetPos();
                const auto vehicleRot = trackingData->vehicle->GetRot();
                interp->interpolator.GetPosition()->SetTargetValue({ vehiclePos.x, vehiclePos.y, vehiclePos.z }, tr->pos, MafiaMP::Core::gApplication->GetTickInterval());
                interp->interpolator.GetRotation()->SetTargetValue({vehicleRot.w,vehicleRot.x, vehicleRot.y, vehicleRot.z}, tr->rot, MafiaMP::Core::gApplication->GetTickInterval());
            } else {
                SDK::ue::sys::math::C_Vector newPos = { tr->pos.x, tr->pos.y, tr->pos.z };
                SDK::ue::sys::math::C_Quat newRot   = { tr->rot.x, tr->rot.y, tr->rot.z, tr->rot.w };
                SDK::ue::sys::math::C_Matrix transform = {};
                transform.Identity();
                transform.SetRot(newRot);
                transform.SetPos(newPos);
                trackingData->vehicle->GetVehicle()->SetVehicleMatrix(transform, SDK::ue::sys::core::E_TransformChangeType::DEFAULT);
            }
        }

        static inline void RemoveVehicle(flecs::entity e) {
            auto trackingData = e.get_mut<Core::Modules::Vehicle::Tracking>();
            if (!trackingData) {
                return;
            }

            Core::gApplication->GetEntityFactory()->ReturnEntity(trackingData->info);
            trackingData->info = nullptr;
        }
    };
} // namespace MafiaMP::Core::Modules
