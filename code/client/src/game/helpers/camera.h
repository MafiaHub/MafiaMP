#pragma once

#include "sdk/ue/sys/math/c_vector.h"

namespace MafiaMP::Game::Helpers {
    class Camera {
      public:
        static bool SetFPV(bool);
        static bool ResetBehindPlayer();

        static bool SimpleShake();
    };
}
