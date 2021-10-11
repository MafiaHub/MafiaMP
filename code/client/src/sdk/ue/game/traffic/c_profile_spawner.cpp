#include "c_profile_spawner.h"

#include <utils/hooking/hooking.h>
#include "../../../patterns.h"
#include "../../../c_ie.h"

namespace SDK {
    namespace ue::game::traffic {
        C_ProfileSpawner::C_ProfileSpawner(ue::C_WeakPtr<ue::sys::core::C_SceneObject> &sceneObject, const ue::sys::utils::C_HashName &profile, int unk1, unsigned long unk2) {
            (*(void(__thiscall *)(C_ProfileSpawner *, ue::C_WeakPtr<ue::sys::core::C_SceneObject> &, const ue::sys::utils::C_HashName &, int,
                unsigned long))gPatterns.C_ProfileSpawner__C_ProfileSpawnerAddr)(this, sceneObject, profile, unk1, unk2);
        }

        C_ProfileSpawner::~C_ProfileSpawner() {
            hook::this_call<void>(gPatterns.C_ProfileSpawner__C_ProfileSpawnerDctorAddr, this);
        }

        bool C_ProfileSpawner::IsSpawnProfileLoaded() {
            return hook::this_call<bool>(gPatterns.C_ProfileSpawner__IsSpawnProfileLoadedAddr, this);
        }

        void C_ProfileSpawner::CreateActor(ue::C_WeakPtr<ue::sys::core::C_SceneObject> &SceneObject, C_Actor *&pActor, void *unk1, unsigned int unk2, unsigned int unk3) {
            (*(void(__thiscall *)(C_ProfileSpawner *, ue::C_WeakPtr<ue::sys::core::C_SceneObject> &, C_Actor *&, void *, unsigned int,
                unsigned int))gPatterns.C_ProfileSpawner__CreateActorAddr)(this, SceneObject, pActor, unk1, unk2, unk3);
        }

        void C_ProfileSpawner::ReturnObject(ue::sys::core::C_SceneObject *pSceneObject) {
            hook::this_call<void>(gPatterns.C_ProfileSpawner__ReturnObjectAddr, this, pSceneObject);
        }

        void *C_ProfileSpawner::operator new(size_t size) {
            return C_IE::Alloc(size);
        }

        void C_ProfileSpawner::operator delete(void *ptr) {
            C_IE::Free(ptr);
        }
    } // namespace ue::game::traffic
} // namespace SDK
