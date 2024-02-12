#include "c_translocator.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::translocator {
        void C_Translocator::SpawnObject(S_ObjectToSpawn const& object) {
            hook::this_call<void>(gPatterns.C_Translocator__SpawnObject, this, object);
        }

        void C_Translocator::SpawnObjects(ue::C_TimeBudget const& timeBudget) {
            hook::this_call<void>(gPatterns.C_Translocator__SpawnObjects, this, timeBudget);
        }
    }
}
