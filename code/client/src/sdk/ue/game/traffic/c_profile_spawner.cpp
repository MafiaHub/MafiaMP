#include "c_profile_spawner.h"

#include "../../../c_ie.h"
#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::traffic {
        C_ProfileSpawner::C_ProfileSpawner(ue::C_WeakPtr<ue::sys::core::C_SceneObject> sceneObject, const ue::sys::utils::C_HashName &profile, int unk1, unsigned long unk2) {
            (*(void(__thiscall *)(C_ProfileSpawner *, ue::C_WeakPtr<ue::sys::core::C_SceneObject> &, const ue::sys::utils::C_HashName &, int, unsigned long))gPatterns.C_ProfileSpawner__C_ProfileSpawner)(this, sceneObject, profile, unk1, unk2);
        }

        C_ProfileSpawner::~C_ProfileSpawner() {
            hook::this_call<void>(gPatterns.C_ProfileSpawner__C_ProfileSpawnerDctor, this);
        }

        bool C_ProfileSpawner::IsSpawnProfileLoaded() {
            return hook::this_call<bool>(gPatterns.C_ProfileSpawner__IsSpawnProfileLoaded, this);
        }

        void C_ProfileSpawner::CreateActor(ue::C_WeakPtr<ue::sys::core::C_SceneObject> &SceneObject, C_Actor *&pActor, void *unk1, unsigned int unk2, unsigned int unk3) {
            (*(void(__thiscall *)(C_ProfileSpawner *, ue::C_WeakPtr<ue::sys::core::C_SceneObject> &, C_Actor *&, void *, unsigned int, unsigned int))gPatterns.C_ProfileSpawner__CreateActor)(this, SceneObject, pActor, unk1, unk2, unk3);
        }

        void C_ProfileSpawner::ReturnObject(ue::sys::core::C_SceneObject *pSceneObject) {
            hook::this_call<void>(gPatterns.C_ProfileSpawner__ReturnObject, this, pSceneObject);
        }

        void *C_ProfileSpawner::operator new(size_t size) {
            return C_IE::Alloc(size);
        }

        void C_ProfileSpawner::operator delete(void *ptr) {
            C_IE::Free(ptr);
        }
    } // namespace ue::game::traffic
} // namespace SDK
