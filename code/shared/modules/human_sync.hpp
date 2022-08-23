#pragma once

#include <cstdint>

#include <flecs/flecs.h>
#include <glm/glm.hpp>

enum CarEnterStates { STATE_OUTSIDE, STATE_ENTERING, STATE_LEAVING, STATE_INSIDE };

namespace MafiaMP::Shared::Modules {
    struct HumanSync {
        struct UpdateData {
            uint8_t _healthPercent = 100.0f;

            uint8_t _charStateHandlerType = -1;
            uint8_t _moveMode             = -1;

            bool _isSprinting = false;
            bool _isStalking  = false;

            float _sprintSpeed = 0.0f;

            struct CarPassenger {
                CarEnterStates enterState = STATE_OUTSIDE;
                bool enterForced{};
                uint64_t carId{};
                int seatId{};
            } carPassenger{};

            struct WeaponData {
                // TODO
                bool isAiming = false;
                bool isFiring = false;
                glm::vec3 aimPos {};
                glm::vec3 aimDir {};
                uint16_t currentWeaponId {};
            } weaponData;
        };

        HumanSync(flecs::world& world) {
            world.module<HumanSync>();

            world.component<UpdateData>();
        }
    };
}
