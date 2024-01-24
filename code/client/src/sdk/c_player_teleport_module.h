#pragma once

#include "patterns.h"

#include "c_ticked_module.h"

#include "ue/c_cnt_ptr.h"
#include "ue/sys/math/c_vector.h"

namespace SDK {
    class C_PlayerTeleportModule: public C_TickedModule {
      public:
        C_PlayerTeleportModule *TeleportPlayer(ue::C_CntPtr<uintptr_t> &, ue::sys::math::C_Vector const &, ue::sys::math::C_Vector const &, bool, bool, bool, bool);

        static C_PlayerTeleportModule *GetInstance();
    };

    C_PlayerTeleportModule *GetPlayerTeleportModule();
} // namespace SDK
