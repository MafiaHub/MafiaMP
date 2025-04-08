#pragma once

#include "c_player_spawner.h"

#include <sdk/c_ticked_module.h>
#include <sdk/ue/game/script/c_sync_object.h>
#include <sdk/ue/sys/utils/c_hash_name.h>

namespace SDK {
    class C_PlayerModelManager : public C_TickedModule {
      public:
        ue::game::script::C_SyncObject *m_pSyncObject;      // 0008 - 0010
        char pad0[0x18];                                    // 0010 - 0028
        C_PlayerSpawner *m_pPlayerSpawner;                  // 0028 - 0030
        char pad1[0x10];                                    // 0030 - 0040
        ue::game::script::C_SyncObject *m_pSyncObject2;     // 0040 - 0048

      public:
        C_PlayerSpawner *GetPlayerSpawner() {
            return m_pPlayerSpawner;
        }
        bool IsPlayerLoaded();

        void *SwitchSpawnProfile(ue::sys::utils::C_HashName);
    };
}
