#pragma once

#include "sdk/ue/sys/core/i_core.h"
#include "sdk/ue/sys/core/c_core.h"
#include "sdk/ue/sys/core/c_frame.h"

namespace MafiaMP::Game::Entities {
    class Factory {
      public:
        static SDK::ue::sys::core::C_Frame *AllocateFrame();
    };
}
