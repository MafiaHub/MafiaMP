#pragma once

#include <cstdint>

#include "ue/sys/math/c_vector.h"
#include "ue/c_cnt_ptr.h"

namespace SDK {
    class C_PlayerTeleportModule {
      public:
        static C_PlayerTeleportModule *GetInstance();

        void TeleportPlayer(ue::C_CntPtr<uintptr_t> &, ue::sys::math::C_Vector const &, ue::sys::math::C_Vector const &, bool, bool, bool, bool);
    };

    C_PlayerTeleportModule *GetPlayerTeleportModule();
}
