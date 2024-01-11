#pragma once

#include <cstdint>

#include <flecs/flecs.h>

#include <glm/glm.hpp>

namespace MafiaMP::Shared::Modules {
    struct VehicleSync {
        enum class LockState {
            Unlocked,
            Locked,
            Breakable,
        };

        struct UpdateData {
            glm::vec3 velocity {};
            glm::vec3 angularVelocity {};
            bool beaconLightsState = false;
            float brake            = 0.0f;
            int gear               = 0;
            float handbrake        = 0.0f;
            bool hornState         = false;
            char licensePlate[7]   = "";
            LockState locked       = LockState::Unlocked;
            float power            = 0.0f;
            int radioId            = 0;
            bool radioState        = 0;
            bool sirenState        = false;
            float steer            = 0.0f;
        };

        VehicleSync(flecs::world &world) {
            world.module<VehicleSync>();

            world.component<UpdateData>();
        }
    };
} // namespace MafiaMP::Shared::Modules
