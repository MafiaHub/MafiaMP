#include "c_player_model_manager.h"

#include "../../patterns.h"

namespace SDK {
    bool C_PlayerModelManager::IsPlayerLoaded() {
        return hook::this_call<bool>(gPatterns.C_PlayerModelManager__IsPlayerLoaded, this);
    }

    void *C_PlayerModelManager::SwitchSpawnProfile(ue::sys::utils::C_HashName name) {
        void *pSyncObject;
        return hook::this_call<void *>(gPatterns.C_PlayerModelManager__SwitchPlayerProfile, this, &pSyncObject, name);
    }
} // namespace SDK
