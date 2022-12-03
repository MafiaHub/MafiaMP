#pragma once

#include "c_rigid_body.h"

namespace SDK {
    namespace ue::game::physics {
        class C_AgentController {
          public:
            char pad0[0x80];                // 0000 - 0080
            C_RigidBody *m_pRigidBody;      // 0080 - 0088
        };
    }
}
