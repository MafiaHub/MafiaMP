#pragma once

#include "c_rigid_body.h"

namespace SDK {
    namespace ue::game::physics {
        class C_AgentController {
          public:
            char pad0[0x34];                // 0000 - 0034
            float m_fVelocity;              // 0034 - 0035
            char pad1[0x75];                // 0035 - 0080
            C_RigidBody *m_pRigidBody;      // 0080 - 0088
        };
    }
}
