#pragma once

#include "sdk/ue/sys/math/c_vector.h"

namespace SDK {
    struct E_HitZone {};
    namespace ue::game::injury {
        class C_InjuryAnalyser {
          public:
            void AddInjury(float, E_HitZone, sys::math::C_Vector const &);
        };
    } // namespace ue::game::human
} // namespace SDK
