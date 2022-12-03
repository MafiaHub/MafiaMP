#pragma once

#include "../physics/c_agent_controller.h"

namespace SDK {
    namespace ue::game::human {
        class C_Agent {
          public:
            char pad0[0x298];                                           // 0000 - 0298
            game::physics::C_AgentController *m_pAgentController;       // 0298 - 02A0
        };
    }
}
