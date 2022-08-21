#pragma once

#include <flecs/flecs.h>

#include "game/streaming/entity_factory.h"
#include "sdk/entities/c_car.h"
#include "sdk/c_game.h"

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

        static void Remove(flecs::entity e);

        static void SetupMessages(Application *app);
        
        static flecs::entity_t GetCarEntity(SDK::C_Car *carPtr);
    };
} // namespace MafiaMP::Core::Modules
