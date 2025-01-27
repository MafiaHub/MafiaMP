#pragma once

#include <cstdint>

#include "sdk/ue/sys/math/c_vector.h"

namespace SDK {
    namespace ue::game::camera {
        struct PointAtVecCommand {
            ue::sys::math::C_Vector targetPosition; // 0x00
            ue::sys::math::C_Vector upVector;       // 0x0C
            uint64_t flags;                         // 0x18
            void *callback;                         // 0x20
            uint64_t callbackData;                  // 0x28
            void *handle;                           // 0x30
            uint64_t someData;                      // 0x38
            uint16_t lockFlags;                     // 0x40
            float rotationParams[3];                // 0x44
            float lockTime;                         // 0x50
            uint64_t unknown;                       // 0x58
        };
    }
}
