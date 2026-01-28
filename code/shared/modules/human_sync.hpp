#pragma once

#include <cstdint>
#include <array>

#include <flecs/flecs.h>
#include <glm/glm.hpp>


namespace MafiaMP::Shared::Modules {
    // Number of animation variables to sync
    // We sync the most important ones for visual fidelity
    static constexpr size_t WANIM_VAR_SYNC_COUNT = 39;

    struct HumanSync {
        struct UpdateData {
            float _healthPercent = 100.0f;

            int _charStateHandlerType = -1;
            int _moveMode             = -1;

            bool _isSprinting = false;
            bool _isStalking  = false;

            float _sprintSpeed = 0.0f;

            // All animation behavior variables (E_WAnimBehaviorVar)
            // Index corresponds to E_WAnimBehaviorVar enum value
            std::array<float, WANIM_VAR_SYNC_COUNT> _animVars = {};

            struct CarPassenger {
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
