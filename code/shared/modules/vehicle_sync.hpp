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
            glm::vec3 angularVelocity {};
            bool beaconLightsOn = false;
            float brake         = 0.0f;
            glm::vec4 colorPrimary {1.0f, 1.0f, 1.0f, 1.0f};
            glm::vec4 colorSecondary {1.0f, 1.0f, 1.0f, 1.0f};
            float dirt           = 0.0f;
            float fuel           = 100.0f; // We use arbitrary value, the max depends of the vehicle. See C_Motor::GetFuelSettings.
            int gear             = 0;
            float handbrake      = 0.0f;
            bool hornOn          = false;
            char licensePlate[7] = "M1-MP";
            LockState lockState  = LockState::Unlocked;
            float power          = 0.0f;
            bool radioOn         = false;
            int radioStationId   = 0;
            glm::vec4 rimColor {1.0f, 1.0f, 1.0f, 1.0f};
            float rust   = 0.0f;
            bool sirenOn = false;
            float steer  = 0.0f;
            glm::vec4 tireColor {1.0f, 1.0f, 1.0f, 1.0f};
            glm::vec3 velocity {};
            glm::vec4 windowTint {0.0f, 0.0f, 0.0f, 20.0f / 255.0f};
        };

        VehicleSync(flecs::world &world) {
            world.module<VehicleSync>();

            world.component<UpdateData>();
        }
    };
} // namespace MafiaMP::Shared::Modules
