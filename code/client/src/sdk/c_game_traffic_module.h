#pragma once

#include "ue/game/traffic/c_race_manager.h"
#include "c_ticked_module.h"

namespace SDK {
    class C_GameTrafficModule : public C_TickedModule {
        public:
        char pad0[0xA0];                                    // 0000 - 00A0
        ue::game::traffic::C_RaceManager *m_pRaceManager;   // 00A0 - 00A8

        public:
        static C_GameTrafficModule *GetInstance();
    };
}
