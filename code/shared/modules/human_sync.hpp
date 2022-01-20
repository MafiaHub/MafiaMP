#pragma once

#include <cstdint>

#include <flecs/flecs.h>

namespace MafiaMP::Shared::Modules {
    struct HumanSync {
        struct TrackingMetadata {
            uint8_t _healthPercent = 100.0f;

            uint8_t _charStateHandlerType = -1;
            uint8_t _moveMode             = -1;

            bool _isSprinting = false;
            bool _isStalking  = false;

            float _sprintSpeed = 0.0f;
        };

        HumanSync(flecs::world& world) {
            world.module<HumanSync>();

            world.component<TrackingMetadata>();
        }
    };
}
