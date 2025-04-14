#pragma once

#include "c_translocable_entity_cache.h"
#include "s_translocable_object.h"
#include <sdk/ue/c_time_budget.h>

namespace SDK {
    namespace ue::game::translocator {
        class I_TranslocableObjectSpawner {
          public:
            virtual ~I_TranslocableObjectSpawner() = default;
        };

        class C_TranslocableObjectSpawner : public I_TranslocableObjectSpawner {
          public:
            void BeginUpdate(ue::C_TimeBudget &);

            void EndUpdate(ue::C_TimeBudget &);

            void Spawn(unsigned int, S_TranslocableObject const &, bool);

            void Update(float);
        };
    }
}
