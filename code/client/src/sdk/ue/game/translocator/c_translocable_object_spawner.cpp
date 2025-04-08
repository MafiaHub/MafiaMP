#include "c_translocable_object_spawner.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::translocator {
        void C_TranslocableObjectSpawner::BeginUpdate(ue::C_TimeBudget &budget) {
            hook::this_call(gPatterns.C_TranslocableObjectSpawner__BeginUpdate, this, budget);
        }

        void C_TranslocableObjectSpawner::EndUpdate(ue::C_TimeBudget &budget) {
            hook::this_call(gPatterns.C_TranslocableObjectSpawner__EndUpdate, this, budget);
        }

        void C_TranslocableObjectSpawner::Spawn(unsigned int unk, S_TranslocableObject const &object, bool unk2) {
            hook::this_call(gPatterns.C_TranslocableObjectSpawner__Spawn, this, unk, object, unk2);
        }

        void C_TranslocableObjectSpawner::Update(float tick) {
            hook::this_call(gPatterns.C_TranslocableObjectSpawner__Update, this, tick);
        }
    }
}
