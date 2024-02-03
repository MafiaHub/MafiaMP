#include <utils/safe_win32.h>

#include "vehicle.h"

#include "sdk/entities/c_vehicle.h"
#include "sdk/mafia/framework/c_mafia_framework.h"
#include "sdk/mafia/framework/c_mafia_framework_interfaces.h"

#include "game/streaming/entity_tracking_info.h"

#include <utility>
#include <world/modules/base.hpp>

#include "shared/messages/vehicle/vehicle_despawn.h"
#include "shared/messages/vehicle/vehicle_owner_update.h"
#include "shared/messages/vehicle/vehicle_spawn.h"
#include "shared/messages/vehicle/vehicle_update.h"

#include "shared/game_rpc/vehicle/vehicle_setprops.h"

#include "shared/modules/mod.hpp"
#include "shared/modules/vehicle_sync.hpp"

namespace MafiaMP::Core::Modules {
    flecs::query<Vehicle::Tracking> Vehicle::_findAllVehicles {};

    Vehicle::Vehicle(flecs::world &world) {
        world.module<Vehicle>();

        world.component<Tracking>();
        world.component<Interpolated>();

        _findAllVehicles = world.query_builder<Tracking>().build();

        world.system<Tracking, Shared::Modules::VehicleSync::UpdateData, Framework::World::Modules::Base::Transform>("UpdateOwnedVehicles")
            .each([](flecs::entity e, Tracking &tracking, Shared::Modules::VehicleSync::UpdateData &metadata, Framework::World::Modules::Base::Transform &tr) {
                const auto myGUID = Core::gApplication->GetNetworkingEngine()->GetNetworkClient()->GetPeer()->GetMyGUID();
                if (tracking.car && Framework::World::Engine::IsEntityOwner(e, myGUID.g)) {
                    const auto car = tracking.car;

                    SDK::ue::sys::math::C_Vector carPos = ((SDK::C_Actor *)car)->GetPos();
                    SDK::ue::sys::math::C_Quat carRot   = ((SDK::C_Actor *)car)->GetRot();
                    SDK::C_Vehicle *vehicle             = car->GetVehicle();

                    SDK::ue::sys::math::C_Vector vehicleVelocity        = vehicle->GetSpeed();
                    SDK::ue::sys::math::C_Vector vehicleAngularVelocity = vehicle->GetAngularSpeed();

                    SDK::ue::sys::math::C_Vector4 colorPrimary, colorSecondary;
                    vehicle->GetVehicleColor(&colorPrimary, &colorSecondary);

                    SDK::ue::sys::math::C_Vector4 rimColor, tireColor;
                    vehicle->GetWheelColor(&rimColor, &tireColor);

                    SDK::ue::sys::math::C_Vector4 windowTint = vehicle->GetWindowTintColor();

                    tr.pos = {carPos.x, carPos.y, carPos.z};
                    tr.rot = {carRot.w, carRot.x, carRot.y, carRot.z};

                    metadata.angularVelocity = {vehicleAngularVelocity.x, vehicleAngularVelocity.y, vehicleAngularVelocity.z};
                    metadata.beaconLightsOn  = vehicle->GetBeaconLightsOn();
                    metadata.brake           = vehicle->GetBrake();
                    metadata.colorPrimary    = {colorPrimary.r, colorPrimary.g, colorPrimary.b, colorPrimary.a};
                    metadata.colorSecondary  = {colorSecondary.r, colorSecondary.g, colorSecondary.b, colorSecondary.a};
                    metadata.dirt            = vehicle->GetVehicleDirty();
                    metadata.fuel            = car->GetActualFuel();
                    metadata.gear            = car->GetGear();
                    metadata.handbrake       = vehicle->GetHandbrake();
                    metadata.hornOn          = vehicle->GetHorn();
                    metadata.power           = vehicle->GetPower();
                    metadata.radioOn         = vehicle->IsRadioOn();
                    metadata.radioStationId  = vehicle->GetRadioStation();
                    metadata.rimColor        = {rimColor.r, rimColor.g, rimColor.b, rimColor.a};
                    metadata.rust            = vehicle->GetVehicleRust();
                    metadata.sirenOn         = vehicle->GetSiren();
                    metadata.steer           = vehicle->GetSteer();
                    metadata.tireColor       = {tireColor.r, tireColor.g, tireColor.b, tireColor.a};
                    metadata.velocity        = {vehicleVelocity.x, vehicleVelocity.y, vehicleVelocity.z};
                    metadata.windowTint      = {windowTint.r, windowTint.g, windowTint.b, windowTint.a};
                }
            });

        world.system<Tracking, Interpolated>("UpdateRemoteVehicles").each([](flecs::entity e, Tracking &tracking, Interpolated &interpolated) {
            const auto myGUID = Core::gApplication->GetNetworkingEngine()->GetNetworkClient()->GetPeer()->GetMyGUID();
            if (tracking.car && !Framework::World::Engine::IsEntityOwner(e, myGUID.g)) {
                const auto car                         = tracking.car;
                const auto vehiclePos                  = car->GetPos();
                const auto vehicleRot                  = car->GetRot();
                const auto newPos                      = interpolated.interpolator.GetPosition()->UpdateTargetValue({vehiclePos.x, vehiclePos.y, vehiclePos.z});
                const auto newRot                      = interpolated.interpolator.GetRotation()->UpdateTargetValue({vehicleRot.w, vehicleRot.x, vehicleRot.y, vehicleRot.z});
                SDK::ue::sys::math::C_Matrix transform = {};
                transform.Identity();
                transform.SetRot({newRot.x, newRot.y, newRot.z, newRot.w});
                transform.SetPos({newPos.x, newPos.y, newPos.z});
                car->GetVehicle()->SetVehicleMatrix(transform, SDK::ue::sys::core::E_TransformChangeType::DEFAULT);
            }
        });
    }

    void Vehicle::Create(flecs::entity e, std::string modelName) {
        auto info          = Core::gApplication->GetEntityFactory()->RequestVehicle(std::move(modelName));
        auto trackingData  = e.get_mut<Core::Modules::Vehicle::Tracking>();
        trackingData->info = info;
        trackingData->car  = nullptr;

        auto interp = e.get_mut<Interpolated>();
        interp->interpolator.GetPosition()->SetCompensationFactor(1.5f);

        e.set<Shared::Modules::Mod::EntityKind>({Shared::Modules::Mod::MOD_VEHICLE});

        const auto OnVehicleRequestFinish = [](Game::Streaming::EntityTrackingInfo *info, bool success) {
            if (success) {
                auto car = reinterpret_cast<SDK::C_Car *>(info->GetEntity());
                if (!car) {
                    return;
                }
                car->GameInit();
                car->Activate();
                car->Unlock();

                const auto ent                         = info->GetNetworkEntity();
                const auto tr                          = ent.get<Framework::World::Modules::Base::Transform>();
                SDK::ue::sys::math::C_Quat newRot      = {tr->rot.x, tr->rot.y, tr->rot.z, tr->rot.w};
                SDK::ue::sys::math::C_Vector newPos    = {tr->pos.x, tr->pos.y, tr->pos.z};
                SDK::ue::sys::math::C_Matrix transform = {};
                transform.Identity();
                transform.SetRot(newRot);
                transform.SetPos(newPos);
                car->GetVehicle()->SetVehicleMatrix(transform, SDK::ue::sys::core::E_TransformChangeType::DEFAULT);

                auto trackingData = ent.get_mut<Core::Modules::Vehicle::Tracking>();
                trackingData->car = car;

                auto streamable                  = ent.get_mut<Framework::World::Modules::Base::Streamable>();
                streamable->modEvents.updateProc = [](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                    const auto updateData = e.get<Shared::Modules::VehicleSync::UpdateData>();
                    const auto serverID   = e.get<Framework::World::Modules::Base::ServerID>();

                    Shared::Messages::Vehicle::VehicleUpdate vehicleUpdate {};
                    vehicleUpdate.SetServerID(serverID->id);
                    vehicleUpdate.SetData(*updateData);
                    peer->Send(vehicleUpdate, guid);
                    return true;
                };

                Update(ent);
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

    void Vehicle::Update(flecs::entity e) {
        const auto trackingData = e.get<Core::Modules::Vehicle::Tracking>();
        if (!trackingData || !trackingData->car) {
            return;
        }

        // Update basic data
        const auto tr = e.get<Framework::World::Modules::Base::Transform>();
        auto interp   = e.get_mut<Interpolated>();
        if (interp) {
            const auto vehicleRot = trackingData->car->GetRot();
            const auto vehiclePos = trackingData->car->GetPos();
            interp->interpolator.GetRotation()->SetTargetValue({vehicleRot.w, vehicleRot.x, vehicleRot.y, vehicleRot.z}, tr->rot, MafiaMP::Core::gApplication->GetTickInterval());
            interp->interpolator.GetPosition()->SetTargetValue({vehiclePos.x, vehiclePos.y, vehiclePos.z}, tr->pos, MafiaMP::Core::gApplication->GetTickInterval());
        }
        else {
            SDK::ue::sys::math::C_Quat newRot   = {tr->rot.x, tr->rot.y, tr->rot.z, tr->rot.w};
            SDK::ue::sys::math::C_Vector newPos = {tr->pos.x, tr->pos.y, tr->pos.z};
            trackingData->car->SetRot(newRot);
            trackingData->car->SetPos(newPos);
        }

        auto updateData         = e.get_mut<Shared::Modules::VehicleSync::UpdateData>();
        SDK::C_Car *car         = trackingData->car;
        SDK::C_Vehicle *vehicle = car->GetVehicle();

        SDK::ue::sys::math::C_Vector4 colorPrimary   = {updateData->colorPrimary.r, updateData->colorPrimary.g, updateData->colorPrimary.b, updateData->colorPrimary.a};
        SDK::ue::sys::math::C_Vector4 colorSecondary = {updateData->colorSecondary.r, updateData->colorSecondary.g, updateData->colorSecondary.b, updateData->colorSecondary.a};

        SDK::ue::sys::math::C_Vector4 rimColor  = {updateData->rimColor.r, updateData->rimColor.g, updateData->rimColor.b, updateData->rimColor.a};
        SDK::ue::sys::math::C_Vector4 tireColor = {updateData->tireColor.r, updateData->tireColor.g, updateData->tireColor.b, updateData->tireColor.a};

        SDK::ue::sys::math::C_Vector4 windowTint = {updateData->windowTint.r, updateData->windowTint.g, updateData->windowTint.b, updateData->windowTint.a};

        vehicle->SetAngularSpeed({updateData->angularVelocity.x, updateData->angularVelocity.y, updateData->angularVelocity.z}, false);
        vehicle->SetBeaconLightsOn(updateData->beaconLightsOn);
        vehicle->SetBrake(updateData->brake, false);
        vehicle->SetVehicleColor(&colorPrimary, &colorSecondary, false);
        car->SetVehicleDirty(updateData->dirt); // We have to use the car to set the dirt otherwise the value is reset
        car->SetActualFuel(updateData->fuel);
        vehicle->SetGear(updateData->gear);
        vehicle->SetHandbrake(updateData->handbrake, false);
        vehicle->SetHorn(updateData->hornOn);
        if (strcmp(vehicle->GetSPZText(), updateData->licensePlate)) {
            vehicle->SetSPZText(updateData->licensePlate, true);
        }
        vehicle->SetPower(updateData->power);
        if (vehicle->IsRadioOn() != updateData->radioOn) {
            vehicle->TurnRadioOn(updateData->radioOn);
        }
        if (vehicle->GetRadioStation() != updateData->radioStationId) {
            vehicle->ChangeRadioStation(updateData->radioStationId);
        }
        vehicle->SetSiren(updateData->sirenOn);
        vehicle->SetVehicleRust(updateData->rust);
        vehicle->SetSpeed({updateData->velocity.x, updateData->velocity.y, updateData->velocity.z}, false, false);
        vehicle->SetSteer(updateData->steer);
        vehicle->SetWheelColor(&rimColor, &tireColor);
        vehicle->SetWindowTintColor(windowTint);
    }

    void Vehicle::SelfUpdate(flecs::entity e, MafiaMP::Shared::Modules::VehicleSync::UpdateData &updateData) {
        // TODO: deprecate in favor of RPCs
    }

    void Vehicle::Remove(flecs::entity e) {
        auto trackingData = e.get_mut<Core::Modules::Vehicle::Tracking>();
        if (!trackingData) {
            return;
        }

        Core::gApplication->GetEntityFactory()->ReturnEntity(trackingData->info);
        trackingData->info = nullptr;
    }

    void Vehicle::SetupMessages(Application *app) {
        const auto net = app->GetNetworkingEngine()->GetNetworkClient();
        net->RegisterMessage<Shared::Messages::Vehicle::VehicleSpawn>(Shared::Messages::ModMessages::MOD_VEHICLE_SPAWN, [app](SLNet::RakNetGUID guid, Shared::Messages::Vehicle::VehicleSpawn *msg) {
            auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            // Setup tracking info
            Create(e, msg->GetModelName());

            // Setup other components
            auto vehicleData = e.get_mut<Shared::Modules::VehicleSync::UpdateData>();
            *vehicleData     = msg->GetSpawnData();
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
            *updateData     = msg->GetData();
            Update(e);
        });
        // TODO: deprecate in favor of RPCs
        /*net->RegisterMessage<Shared::Messages::Vehicle::VehicleOwnerUpdate>(Shared::Messages::ModMessages::MOD_VEHICLE_OWNER_UPDATE, [app](SLNet::RakNetGUID guid, Shared::Messages::Vehicle::VehicleOwnerUpdate *msg) {
            const auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }
            SelfUpdate(e, msg->GetData());
        });*/

        InitRPCs(app);
    }

    void Vehicle::InitRPCs(Application *app) {
        const auto net = app->GetNetworkingEngine()->GetNetworkClient();

        net->RegisterGameRPC<Shared::RPC::VehicleSetProps>([app](SLNet::RakNetGUID guid, Shared::RPC::VehicleSetProps *msg) {
            const auto e = app->GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            auto updateData = e.get_mut<Shared::Modules::VehicleSync::UpdateData>();

            const auto beaconLightsOn = msg->beaconLightsOn;
            const auto colorPrimary   = msg->colorPrimary;
            const auto colorSecondary = msg->colorSecondary;
            const auto dirt           = msg->dirt;
            const auto fuel           = msg->fuel;
            const auto licensePlate   = msg->licensePlate;
            const auto lockState      = msg->lockState;
            const auto radioOn        = msg->radioOn;
            const auto radioStationId = msg->radioStationId;
            const auto rimColor       = msg->rimColor;
            const auto rust           = msg->rust;
            const auto sirenOn        = msg->sirenOn;
            const auto tireColor      = msg->tireColor;
            const auto windowTint     = msg->windowTint;

            if (beaconLightsOn.HasValue()) {
                updateData->beaconLightsOn = beaconLightsOn();
            }

            if (colorPrimary.HasValue()) {
                updateData->colorPrimary = colorPrimary();
            }

            if (colorSecondary.HasValue()) {
                updateData->colorSecondary = colorSecondary();
            }

            if (dirt.HasValue()) {
                updateData->dirt = dirt();
            }

            if (fuel.HasValue()) {
                updateData->fuel = fuel();
            }

            if (licensePlate.HasValue()) {
                const auto plate = licensePlate().C_String();
                ::memcpy(updateData->licensePlate, plate, strlen(plate) + 1);
            }

            if (lockState.HasValue()) {
                updateData->lockState = lockState();
            }

            if (radioOn.HasValue()) {
                updateData->radioOn = radioOn();
            }

            if (radioStationId.HasValue()) {
                updateData->radioStationId = radioStationId();
            }

            if (rimColor.HasValue()) {
                updateData->rimColor = rimColor();
            }

            if (rust.HasValue()) {
                updateData->rust = rust();
            }

            if (sirenOn.HasValue()) {
                updateData->sirenOn = sirenOn();
            }

            if (tireColor.HasValue()) {
                updateData->tireColor = tireColor();
            }

            if (windowTint.HasValue()) {
                updateData->windowTint = windowTint();
            }

            Update(e);
        });
    }

    void Vehicle::UpdateTransform(flecs::entity e) {
        const auto trackingData = e.get<Core::Modules::Vehicle::Tracking>();
        if (!trackingData || !trackingData->car) {
            return;
        }

        // Update basic data
        const auto tr = e.get<Framework::World::Modules::Base::Transform>();
        auto interp   = e.get_mut<Interpolated>();
        if (interp) {
            // TODO reset lerp
            const auto vehicleRot = trackingData->car->GetRot();
            const auto vehiclePos = trackingData->car->GetPos();
            interp->interpolator.GetRotation()->SetTargetValue({vehicleRot.w, vehicleRot.x, vehicleRot.y, vehicleRot.z}, tr->rot, MafiaMP::Core::gApplication->GetTickInterval());
            interp->interpolator.GetPosition()->SetTargetValue({vehiclePos.x, vehiclePos.y, vehiclePos.z}, tr->pos, MafiaMP::Core::gApplication->GetTickInterval());
        }

        SDK::ue::sys::math::C_Quat newRot   = {tr->rot.x, tr->rot.y, tr->rot.z, tr->rot.w};
        SDK::ue::sys::math::C_Vector newPos = {tr->pos.x, tr->pos.y, tr->pos.z};
        trackingData->car->SetRot(newRot);
        trackingData->car->SetPos(newPos);
    }

    flecs::entity Vehicle::GetCarEntity(SDK::C_Car *carPtr) {
        flecs::entity carID {};
        _findAllVehicles.each([&carID, carPtr](flecs::entity e, Core::Modules::Vehicle::Tracking &trackingData) {
            if (trackingData.car == carPtr) {
                carID = e;
            }
        });
        return carID;
    }

    flecs::entity Vehicle::GetCarEntityByVehicle(SDK::C_Vehicle *vehiclePtr) {
        flecs::entity carID {};
        _findAllVehicles.each([&carID, vehiclePtr](flecs::entity e, Core::Modules::Vehicle::Tracking &trackingData) {
            if (trackingData.car && trackingData.car->GetVehicle() && trackingData.car->GetVehicle() == vehiclePtr) {
                carID = e;
            }
        });
        return carID;
    }
} // namespace MafiaMP::Core::Modules
