#pragma once

#include "../../sdk/ue/sys/math/c_vector.h"

namespace MafiaMP::Game::Helpers {
    class Camera {
      public:
        static bool SetPos(SDK::ue::sys::math::C_Vector, SDK::ue::sys::math::C_Vector, bool);
        static bool ResetBehindPlayer();
        
        static bool SimpleShake();
    };
}
