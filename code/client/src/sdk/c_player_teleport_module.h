#pragma once

#include <cstdint>

#include "ue/sys/math/c_vector.h"

namespace SDK {
    class C_PlayerTeleportModule {
      public:
        static C_PlayerTeleportModule *GetInstance();

        void TeleportPlayer(uint64_t *, ue::sys::math::C_Vector const &, ue::sys::math::C_Vector const &, bool, bool, bool);
    };
}
