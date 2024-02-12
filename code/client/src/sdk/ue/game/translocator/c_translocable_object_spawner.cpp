#include "c_translocable_object_spawner.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::translocator {
        void C_TranslocableObjectSpawner::Spawn(unsigned int unk, S_TranslocableObject const &object, bool unk2) {
            hook::this_call<void>(gPatterns.C_TranslocableObjectSpawner__Spawn, this, unk, object, unk2);
        }
    }
}
