#include "c_shot_manager.h"

#include <utils/hooking/hooking.h>
#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::shotmanager {
        C_ShotManager *C_ShotManager::GetInstance() {
            return hook::call<C_ShotManager*>(gPatterns.C_ShotManager__GetInstance);
        }

        C_Fire* C_ShotManager::CreateFire(S_FireInit const* params) {
            return hook::this_call<C_Fire *>(gPatterns.C_ShotManager__CreateFire, this, params);
        }

        C_Explosion* C_ShotManager::CreateExplosion(S_ExplosionInit const* params) {
            return hook::this_call<C_Explosion *>(gPatterns.C_ShotManager__CreateExplosion, this, params);
        }
    }
}
