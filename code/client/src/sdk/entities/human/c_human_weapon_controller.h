#pragma once

#include <cstdint>

#include "../../ue/sys/math/c_vector.h"

namespace SDK {
    class C_HumanWeaponController {
      public:
        int GetRightHandWeaponID();

        void GetShotPosDir(SDK::ue::sys::math::C_Vector *OutPos, SDK::ue::sys::math::C_Vector *OutDir, float *Unknown);

        bool IsThrownWeapon();
    };
} // namespace SDK
