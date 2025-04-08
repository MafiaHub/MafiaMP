#pragma once

#include "c_ticked_module.h"

#include "sdk/ue/game/input/c_game_input.h"
#include "sdk/ue/sys/input/c_sys_input.h"

namespace SDK {
    class C_GameInputModule: public C_TickedModule {
      public:
        char pad0[0x10];                            // 0008 - 0018
        ue::game::input::C_GameInput *m_pGameInput; // 0018 - 0020
        ue::sys::input::C_SysInput *m_pSysInput;    // 0020 - 0028

      public:
        void PauseInput(bool);
    };

    C_GameInputModule* GetGameInputModule();
}
