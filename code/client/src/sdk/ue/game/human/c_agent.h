#pragma once

#include "c_agent_anim.h"
#include "c_agent_phys.h"

#include "../physics/c_agent_controller.h"
#include "../physics/c_game_physics_object.h"

namespace SDK {
    namespace ue::game::human {
        class C_Agent {
          public:
            char pad0[0x20];                                            // 0000 - 0020
            C_AgentAnim *m_pAgentAnim;                                  // 0020 - 0028
            char pad1[0x230];                                           // 0028 - 0258
            C_AgentPhys *m_pAgentPhys;                                  // 0258 - 0260
            char pad2[0x38];                                            // 0260 - 0298
            game::physics::C_AgentController *m_pAgentController;       // 0298 - 02A0
            char pad3[0x18];                                            // 02A0 - 02B8
            game::physics::C_GamePhysicsObject *_pGamePhysicsObject;    // 02B8 - 02C0
        };
    }
}
