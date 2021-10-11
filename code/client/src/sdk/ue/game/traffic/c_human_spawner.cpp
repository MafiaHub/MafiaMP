#include "c_human_spawner.h"

#include "../../../patterns.h"

#include <utils/hooking/hooking.h>

namespace SDK {
    namespace ue::game::traffic {
        C_HumanSpawner::C_HumanSpawner(const ue::sys::utils::C_HashName &profile, int unk): C_ProfileSpawner(SetupDefaultArchetypeObject(), profile, unk) {
            vftable = gPatterns.C_HumanSpawner__C_HumanSpawnerVtblAddr;
        }

        C_HumanSpawner::~C_HumanSpawner() {
            vftable = gPatterns.C_HumanSpawner__C_HumanSpawnerVtblAddr;
        }

        ue::C_WeakPtr<ue::sys::core::C_SceneObject> C_HumanSpawner::SetupDefaultArchetypeObject() {
            return (
                *(ue::C_WeakPtr<ue::sys::core::C_SceneObject>(__thiscall *)(ue::game::traffic::C_HumanSpawner *))gPatterns.C_HumanSpawner__SetupDefaultArchetypeObjectAddr)(this);
        }
    } // namespace ue::game::traffic
} // namespace SDK
