#pragma once

#include "c_player_spawner.h"

#include <sdk/c_ticked_module.h>
#include <sdk/ue/sys/utils/c_hash_name.h>

namespace SDK {
    class C_PlayerModelManager : public C_TickedModule {
      public:
        char pad0[0x20];                        // 0008 - 0028
        C_PlayerSpawner *m_pPlayerSpawner;      // 0028 - 0030

      public:
        C_PlayerSpawner *GetPlayerSpawner() {
            return m_pPlayerSpawner;
        }
        bool IsPlayerLoaded();

        void *SwitchSpawnProfile(ue::sys::utils::C_HashName);
    };
}
