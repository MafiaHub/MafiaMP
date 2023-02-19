#pragma once

#include <flecs/flecs.h>

#include "game/streaming/entity_factory.h"
#include "sdk/entities/c_car.h"
#include "sdk/c_game.h"

#include "shared/modules/vehicle_sync.hpp"

#include <utils/interpolator.h>

#include "core/application.h"

namespace MafiaMP::Core::Modules {
    struct Vehicle {
        struct Tracking {
            SDK::C_Car *car                           = nullptr;
            Game::Streaming::EntityTrackingInfo *info = nullptr;
        };

        struct Interpolated {
            Framework::Utils::Interpolator interpolator = {};
        };

        static flecs::query<Tracking> _findAllVehicles;

        Vehicle(flecs::world &world);

        static void Create(flecs::entity e, std::string modelName);

        static void Update(flecs::entity e);
        static void SelfUpdate(flecs::entity e, MafiaMP::Shared::Modules::VehicleSync::UpdateData &);

        static void Remove(flecs::entity e);

        static void SetupMessages(Application *app);

        static void InitRPCs(Application *app);

        static void UpdateTransform(flecs::entity e);
        
        static flecs::entity GetCarEntity(SDK::C_Car *carPtr);
        static flecs::entity GetCarEntityByVehicle(SDK::C_Vehicle *vehiclePtr);
    };
} // namespace MafiaMP::Core::Modules
