#pragma once

#include <cstdint>

#include <flecs/flecs.h>

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
                uint64_t carId{};
                int seatId{};
            } carPassenger{};
        };

        HumanSync(flecs::world& world) {
            world.module<HumanSync>();

            world.component<UpdateData>();
        }
    };
}
