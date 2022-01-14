#pragma once

#include "c_player_spawner.h"
#include "../../ue/sys/utils/c_hash_name.h"

namespace SDK {
    class C_PlayerModelManager {
      public:
        C_PlayerSpawner *GetPlayerSpawner() {
            return *(C_PlayerSpawner **)(((uintptr_t)this) + 40);
        }
        bool IsPlayerLoaded();

        void *SwitchSpawnProfile(ue::sys::utils::C_HashName);
    };
}
