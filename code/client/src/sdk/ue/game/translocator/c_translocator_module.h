#pragma once

#include "c_translocable_object_spawner.h"
#include "c_translocator.h"

#include "sdk/c_ticked_module.h"

namespace SDK {
    namespace ue::game::translocator {
        class C_TranslocatorModule : public C_TickedModule {
          public:
            ue::game::translocator::C_TranslocableObjectSpawner* m_pTranslocableObjectSpawner;  // 0008 - 0010
            ue::game::translocator::C_Translocator* m_pTranslocator;                            // 0010 - 0018
        };

        C_TranslocatorModule *GetTranslocatorModule();
    }
}
