#include "c_player_model_manager.h"

#include <utils/hooking/hooking.h>
#include "../../patterns.h"

namespace SDK {
    void *C_PlayerModelManager::SwitchSpawnProfile(ue::sys::utils::C_HashName name) {
        void *pSyncObject;
        return hook::this_call<void *>(gPatterns.C_PlayerModelManager__SwitchPlayerProfileAddr, this, &pSyncObject, name);
    }
}
