#include <utils/safe_win32.h>

#include "vehicle.h"

#include "core/application.h"

#include "sdk/ue/game/vehicle/c_vehicle.h"
#include "sdk/mafia/framework/c_mafia_framework.h"
#include "sdk/mafia/framework/c_mafia_framework_interfaces.h"

#include <core_modules.h>
#include <networking/replication/entity_factory.h>
#include <networking/replication/replication_manager.h>
#include <networking/replication/replication_manager.h>

#include <cstring>
#include <utility>

namespace MafiaMP::Core::Modules {
    namespace {
        // Fires when the streamed game car is ready; binds it to its ClientVehicle.
        void OnVehicleRequestFinish(Game::Streaming::EntityTrackingInfo *info, bool success) {
            if (!success) {
                return;
            }
            auto car = reinterpret_cast<SDK::C_Car *>(info->GetEntity());
            if (!car) {
                return;
            }
            car->GameInit();
            car->Activate();
            car->Unlock();

            auto *self = static_cast<Vehicle *>(info->GetNetworkEntity());
            if (!self) {
                return;
            }

            SDK::ue::sys::math::C_Quat newRot      = {self->rotation.x, self->rotation.y, self->rotation.z, self->rotation.w};
            SDK::ue::sys::math::C_Vector newPos    = {self->position.x, self->position.y, self->position.z};
            SDK::ue::sys::math::C_Matrix transform = {};
            transform.Identity();
            transform.SetRot(newRot);
            transform.SetPos(newPos);
            car->GetVehicle()->SetVehicleMatrix(transform, SDK::ue::sys::core::E_TransformChangeType::DEFAULT);
            car->GetVehicle()->SetBrake(self->data.brake, true);

            self->car = car;
            // The replicated state arrived before the game car existed; apply it now. With per-field
            // deltas, unchanged fields are never re-sent, so a late ApplyRemote is the only chance to
            // catch up the car's configuration.
            if (self->IsOwnedByUs()) {
                self->Frame();
            }
            else {
                self->ApplyRemote();
            }
        }

        void OnVehicleReturned(Game::Streaming::EntityTrackingInfo *info, bool wasCreated) {
            if (!info) {
                return;
            }
            auto car = reinterpret_cast<SDK::C_Car *>(info->GetEntity());
            if (wasCreated && car) {
                car->Deactivate();
                car->GameDone();
                car->Release();
            }
        }
    } // namespace

    bool Vehicle::IsOwnedByUs() const {
        const auto *manager = static_cast<const Framework::Networking::Replication::ReplicationManager *>(replicaManager);
        return manager && ownerGUID == manager->GetMyGUID();
    }

    void Vehicle::OnConstructed() {
        info = Core::gApplication->GetEntityFactory()->RequestVehicle(modelName);
        interpolator.GetPosition()->SetCompensationFactor(1.5f);
        info->SetRequestFinishCallback(&OnVehicleRequestFinish);
        info->SetReturnCallback(&OnVehicleReturned);
        info->SetNetworkEntity(this);
    }

    void Vehicle::DeallocReplica(MafiaNet::Connection_RM3 *) {
        if (info) {
            Core::gApplication->GetEntityFactory()->ReturnEntity(info);
            info = nullptr;
        }
        delete this;
    }

    void Vehicle::DeserializeFields(MafiaNet::VariableDeltaSerializer *vds, MafiaNet::VariableDeltaSerializer::DeserializationContext *ctx) {
        VehicleEntity::DeserializeFields(vds, ctx);
        if (!IsOwnedByUs()) {
            ApplyRemote();
        }
    }

    void Vehicle::Frame() {
        if (!car) {
            return;
        }
        if (IsOwnedByUs()) {
            ReadLocal();
        }
        else {
            const auto vehiclePos = car->GetPos();
            const auto vehicleRot = car->GetRot();
            const auto newPos     = interpolator.GetPosition()->UpdateTargetValue({vehiclePos.x, vehiclePos.y, vehiclePos.z});
            const auto newRot     = interpolator.GetRotation()->UpdateTargetValue({vehicleRot.w, vehicleRot.x, vehicleRot.y, vehicleRot.z});

            SDK::ue::sys::math::C_Matrix transform = {};
            transform.Identity();
            transform.SetRot({newRot.x, newRot.y, newRot.z, newRot.w});
            transform.SetPos({newPos.x, newPos.y, newPos.z});
            car->GetVehicle()->SetVehicleMatrix(transform, SDK::ue::sys::core::E_TransformChangeType::DEFAULT);
        }
    }

    void Vehicle::ReadLocal() {
        SDK::ue::sys::math::C_Vector carPos = ((SDK::C_Actor *)car)->GetPos();
        SDK::ue::sys::math::C_Quat carRot   = ((SDK::C_Actor *)car)->GetRot();
        SDK::ue::game::vehicle::C_Vehicle *vehicle = car->GetVehicle();

        SDK::ue::sys::math::C_Vector vehicleVelocity        = vehicle->GetSpeed();
        SDK::ue::sys::math::C_Vector vehicleAngularVelocity = vehicle->GetAngularSpeed();

        SDK::ue::sys::math::C_Vector4 colorPrimary, colorSecondary;
        vehicle->GetVehicleColor(&colorPrimary, &colorSecondary);

        SDK::ue::sys::math::C_Vector4 rimColor, tireColor;
        vehicle->GetWheelColor(&rimColor, &tireColor);

        SDK::ue::sys::math::C_Vector4 windowTint = vehicle->GetWindowTintColor();

        position = {carPos.x, carPos.y, carPos.z};
        rotation = {carRot.w, carRot.x, carRot.y, carRot.z};

        data.angularVelocity = {vehicleAngularVelocity.x, vehicleAngularVelocity.y, vehicleAngularVelocity.z};
        data.beaconLightsOn  = vehicle->GetBeaconLightsOn();
        data.brake           = vehicle->GetBrake();
        data.colorPrimary    = {colorPrimary.r, colorPrimary.g, colorPrimary.b, colorPrimary.a};
        data.colorSecondary  = {colorSecondary.r, colorSecondary.g, colorSecondary.b, colorSecondary.a};
        data.dirt            = vehicle->GetVehicleDirty();
        data.engineOn        = car->IsEngineOn();
        data.fuel            = car->GetActualFuel();
        data.gear            = car->GetGear();
        data.handbrake       = vehicle->GetHandbrake();
        data.hornOn          = vehicle->GetHorn();
        data.power           = vehicle->GetPower();
        data.radioOn         = vehicle->IsRadioOn();
        data.radioStationId  = vehicle->GetRadioStation();
        data.rimColor        = {rimColor.r, rimColor.g, rimColor.b, rimColor.a};
        data.rust            = vehicle->GetVehicleRust();
        data.sirenOn         = vehicle->IsSiren();
        data.steer           = vehicle->GetSteer();
        data.tireColor       = {tireColor.r, tireColor.g, tireColor.b, tireColor.a};
        data.velocity        = {vehicleVelocity.x, vehicleVelocity.y, vehicleVelocity.z};
        data.windowTint      = {windowTint.r, windowTint.g, windowTint.b, windowTint.a};
    }

    void Vehicle::ApplyConfig() {
        if (!car) {
            return;
        }
        SDK::ue::game::vehicle::C_Vehicle *vehicle = car->GetVehicle();

        SDK::ue::sys::math::C_Vector4 colorPrimary   = {data.colorPrimary.r, data.colorPrimary.g, data.colorPrimary.b, data.colorPrimary.a};
        SDK::ue::sys::math::C_Vector4 colorSecondary = {data.colorSecondary.r, data.colorSecondary.g, data.colorSecondary.b, data.colorSecondary.a};
        SDK::ue::sys::math::C_Vector4 rimColor       = {data.rimColor.r, data.rimColor.g, data.rimColor.b, data.rimColor.a};
        SDK::ue::sys::math::C_Vector4 tireColor      = {data.tireColor.r, data.tireColor.g, data.tireColor.b, data.tireColor.a};
        SDK::ue::sys::math::C_Vector4 windowTint     = {data.windowTint.r, data.windowTint.g, data.windowTint.b, data.windowTint.a};

        vehicle->SetBeaconLightsOn(data.beaconLightsOn);
        vehicle->SetVehicleColor(&colorPrimary, &colorSecondary, false);
        car->SetVehicleDirty(data.dirt); // must go through the car or the value resets
        vehicle->SetEngineOn(data.engineOn, data.engineOn);
        car->SetActualFuel(data.fuel);
        if (std::strcmp(vehicle->GetSPZText(), data.licensePlate.data()) != 0) {
            vehicle->SetSPZText(data.licensePlate.data(), true);
        }
        if (vehicle->IsRadioOn() != data.radioOn) {
            vehicle->TurnRadioOn(data.radioOn);
        }
        if (vehicle->GetRadioStation() != data.radioStationId) {
            vehicle->ChangeRadioStation(data.radioStationId);
        }
        vehicle->SetSiren(data.sirenOn);
        vehicle->SetVehicleRust(data.rust);
        vehicle->SetWheelColor(&rimColor, &tireColor);
        vehicle->SetWindowTintColor(windowTint);
    }

    void Vehicle::ApplyRemote() {
        if (!car) {
            return;
        }

        // Set the interpolation target toward the replicated transform.
        const auto vehicleRot = car->GetRot();
        const auto vehiclePos = car->GetPos();
        interpolator.GetRotation()->SetTargetValue({vehicleRot.w, vehicleRot.x, vehicleRot.y, vehicleRot.z}, rotation, MafiaMP::Core::gApplication->GetTickInterval());
        interpolator.GetPosition()->SetTargetValue({vehiclePos.x, vehiclePos.y, vehiclePos.z}, position, MafiaMP::Core::gApplication->GetTickInterval());

        ApplyConfig();

        // Owner-authoritative physics, replicated to the other clients.
        SDK::ue::game::vehicle::C_Vehicle *vehicle = car->GetVehicle();
        vehicle->SetAngularSpeed({data.angularVelocity.x, data.angularVelocity.y, data.angularVelocity.z}, false);
        vehicle->SetBrake(data.brake, false);
        vehicle->SetGear(data.gear);
        vehicle->SetHandbrake(data.handbrake, false);
        vehicle->SetHorn(data.hornOn);
        vehicle->SetPower(data.power);
        vehicle->SetSpeed({data.velocity.x, data.velocity.y, data.velocity.z}, false, false);
        vehicle->SetSteer(data.steer);
    }

    void Vehicle::OnStateForced() {
        // We own this vehicle, so the server withholds normal serialize to us; this is how it gets
        // the last word over our configuration. Apply it to the car — ReadLocal then streams the
        // corrected values back, so there is no fight with the server.
        ApplyConfig();
    }

    void Vehicle::Install() {
        Framework::Networking::Replication::EntityFactory::Get().Register(Shared::Entities::VehicleEntity::kTypeName, [] {
            return new Vehicle();
        });
    }

    void Vehicle::UpdateAll() {
        auto *world = Framework::CoreModules::GetReplication();
        auto *repl  = world;
        if (!repl) {
            return;
        }
        repl->ForEachEntity([](Framework::Networking::Replication::NetworkEntity *e) {
            if (auto *vehicle = dynamic_cast<Vehicle *>(e)) {
                vehicle->Frame();
            }
        });
    }

    Vehicle *Vehicle::GetByCar(SDK::C_Car *carPtr) {
        auto *world = Framework::CoreModules::GetReplication();
        auto *repl  = world;
        if (!repl || !carPtr) {
            return nullptr;
        }
        Vehicle *found = nullptr;
        repl->ForEachEntity([&](Framework::Networking::Replication::NetworkEntity *e) {
            if (found) {
                return;
            }
            if (auto *vehicle = dynamic_cast<Vehicle *>(e); vehicle && vehicle->car == carPtr) {
                found = vehicle;
            }
        });
        return found;
    }

    Vehicle *Vehicle::GetByVehicle(SDK::ue::game::vehicle::C_Vehicle *vehiclePtr) {
        auto *world = Framework::CoreModules::GetReplication();
        auto *repl  = world;
        if (!repl || !vehiclePtr) {
            return nullptr;
        }
        Vehicle *found = nullptr;
        repl->ForEachEntity([&](Framework::Networking::Replication::NetworkEntity *e) {
            if (found) {
                return;
            }
            if (auto *vehicle = dynamic_cast<Vehicle *>(e); vehicle && vehicle->car && vehicle->car->GetVehicle() == vehiclePtr) {
                found = vehicle;
            }
        });
        return found;
    }

    Vehicle *Vehicle::GetByNetworkId(uint64_t networkId) {
        auto *world = Framework::CoreModules::GetReplication();
        return world ? dynamic_cast<Vehicle *>(world->GetEntityByNetworkID(networkId)) : nullptr;
    }
} // namespace MafiaMP::Core::Modules
