#pragma once

#include "s_translocable_object.h"

namespace SDK {
    namespace ue::game::translocator {
        class C_TranslocableObjectSpawner {
          public:
            void Spawn(unsigned int, S_TranslocableObject const &, bool);
        };
    }
}
