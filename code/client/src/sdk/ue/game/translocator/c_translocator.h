#pragma once

#include "../../c_time_budget.h"

namespace SDK {
    namespace ue::game::translocator {
        class C_Translocator {
            struct S_ObjectToSpawn {

            };

            public:
                void SpawnObject(S_ObjectToSpawn const& object);
                void SpawnObjects(ue::C_TimeBudget const& timeBudget);
        };
    }
}
