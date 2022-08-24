#pragma once

#include <cstdint>

#include <flecs/flecs.h>
#include <glm/glm.hpp>


namespace MafiaMP::Shared::Modules {
    struct HumanSync {
        struct UpdateData {
            float _healthPercent = 100.0f;

            int _charStateHandlerType = -1;
            int _moveMode             = -1;

            bool _isSprinting = false;
            bool _isStalking  = false;

            float _sprintSpeed = 0.0f;

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
