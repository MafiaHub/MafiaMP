#pragma once

#include <utils/hooking/hooking.h>

namespace SDK {
    namespace ue::game::camera {
        void ConvertSineWaveShakeEasyVec(sys::math::C_Vector *output, const sys::math::C_Vector *input, bool applyZMod) {
            const auto addr = reinterpret_cast<uint64_t>(hook::get_pattern("F3 0F 10 12 0F 57 C9"));
            hook::call<void>(addr, output, input, applyZMod);
        }
    }
}
