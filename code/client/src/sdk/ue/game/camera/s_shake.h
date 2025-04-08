#pragma once

#include "sdk/ue/sys/math/c_vector.h"
#include "sdk/ue/sys/utils/c_simple_timer.h"

namespace SDK {
    namespace ue::game::camera {
        struct S_Shake {
            ue::sys::math::C_Vector baseAmplitude; // 0x00
            ue::sys::math::C_Vector amplitudeMod1; // 0x0C
            ue::sys::math::C_Vector amplitudeMod2; // 0x18
            ue::sys::math::C_Vector baseFrequency; // 0x24
            ue::sys::math::C_Vector frequencyMod1; // 0x30
            ue::sys::math::C_Vector frequencyMod2; // 0x3C
            ue::sys::math::C_Vector baseDirection; // 0x48
            ue::sys::math::C_Vector directionMod1; // 0x54
            ue::sys::math::C_Vector directionMod2; // 0x60
            sys::utils::C_SimpleTimer timer;       // 0x70
            uint32_t state;                        // 0xC8
            float duration;                        // 0xCC
            float intensity;                       // 0xD0
            float intensityMods[8];                // 0xD4
            uint8_t flags;                         // 0xF4
        };
    }
}
