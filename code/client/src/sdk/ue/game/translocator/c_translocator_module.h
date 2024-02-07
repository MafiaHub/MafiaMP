#pragma once

#include "c_translocable_object_spawner.h"
#include "c_translocator.h"

namespace SDK {
    namespace ue::game::translocator {
        class C_TranslocatorModule {
          private:
            char pad0[0x8];                                                                     // 0000 - 0008
            ue::game::translocator::C_TranslocableObjectSpawner* m_pTranslocableObjectSpawner;  // 0008 - 0010
            ue::game::translocator::C_Translocator* m_pTranslocator;                            // 0010 - 0018
        };
    }
}
