#pragma once

#include <array>
#include <cstdint>
#include <glm/glm.hpp>

#include "shared/constants.h"

namespace MafiaMP::Shared::Modules {
    // Per-tick vehicle state. Each field is delta-tracked individually by VehicleEntity, so changing
    // one property only puts that property on the wire. Every field must therefore be a value type
    // BitStream serializes by value (licensePlate is a std::array, not a char[], so it is not
    // mistaken for a C string).
    struct VehicleSync {
        enum class LockState {
            UNLOCKED,
            LOCKED,
            BREAKABLE,
        };

        struct UpdateData {
            glm::vec3 angularVelocity {};
            bool beaconLightsOn = false;
            float brake         = 1.0f;
            glm::vec4 colorPrimary {1.0f, 1.0f, 1.0f, 1.0f};
            glm::vec4 colorSecondary {1.0f, 1.0f, 1.0f, 1.0f};
            float dirt                                                     = 0.0f;
            bool engineOn                                                  = false;
            float fuel                                                     = 100.0f; // We use arbitrary value, the max depends of the vehicle. See C_Motor::GetFuelSettings.
            int gear                                                       = 0;
            float handbrake                                                = 0.0f;
            bool hornOn                                                                 = false;
            std::array<char, Constants::VEHICLE_LICENSE_PLATE_MAX_LENGTH> licensePlate   = {'A', 'Z', '-', '0', '0', '0'};
            LockState lockState                                                         = LockState::UNLOCKED;
            float power                                                    = 0.0f;
            bool radioOn                                                   = false;
            int radioStationId                                             = 0;
            glm::vec4 rimColor {1.0f, 1.0f, 1.0f, 1.0f};
            float rust   = 0.0f;
            bool sirenOn = false;
            float steer  = 0.0f;
            glm::vec4 tireColor {1.0f, 1.0f, 1.0f, 1.0f};
            glm::vec3 velocity {};
            glm::vec4 windowTint {0.0f, 0.0f, 0.0f, 20.0f / 255.0f};
        };
    };
} // namespace MafiaMP::Shared::Modules
