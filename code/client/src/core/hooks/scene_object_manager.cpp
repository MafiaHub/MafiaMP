#include <MinHook.h>
#include <utils/hooking/hook_function.h>

#include "sdk/patterns.h"
#include "sdk/ue/sys/core/c_scene_object.h"
#include "sdk/ue/c_ptr.h"

#include "core/scene_object_registry.h"

typedef int64_t(__fastcall *C_SceneObjectManager__RegisterSceneObject_t)(void *, SDK::ue::C_Ptr<SDK::ue::sys::core::C_SceneObject> *);
C_SceneObjectManager__RegisterSceneObject_t C_SceneObjectManager__RegisterSceneObject_original = nullptr;
int64_t __fastcall C_SceneObjectManager__RegisterSceneObject(void *pThis, SDK::ue::C_Ptr<SDK::ue::sys::core::C_SceneObject> *pSceneObjectPtr) {
    auto result = C_SceneObjectManager__RegisterSceneObject_original(pThis, pSceneObjectPtr);

    if (pSceneObjectPtr) {
        auto sceneObject = pSceneObjectPtr->Get();
        if (sceneObject) {
            uint64_t nameHash = sceneObject->GetNameHash();
            const char *name  = sceneObject->GetName();
            if (nameHash != 0 && name != nullptr) {
                MafiaMP::Core::gSceneObjectRegistry.Register(nameHash, name);
            }
        }
    }

    return result;
}

typedef void *(__fastcall *C_SceneObjectManager__UnregisterSceneObject_t)(void *, SDK::ue::sys::core::C_SceneObject *);
C_SceneObjectManager__UnregisterSceneObject_t C_SceneObjectManager__UnregisterSceneObject_original = nullptr;
void *__fastcall C_SceneObjectManager__UnregisterSceneObject(void *pThis, SDK::ue::sys::core::C_SceneObject *pSceneObject) {
    if (pSceneObject) {
        uint64_t nameHash = pSceneObject->GetNameHash();

        if (nameHash != 0) {
            MafiaMP::Core::gSceneObjectRegistry.Unregister(nameHash);
        }
    }

    return C_SceneObjectManager__UnregisterSceneObject_original(pThis, pSceneObject);
}

static InitFunction init([]() {
    MH_CreateHook(
        (LPVOID)SDK::gPatterns.C_SceneObjectManager__RegisterSceneObject,
        (PBYTE)C_SceneObjectManager__RegisterSceneObject,
        reinterpret_cast<void **>(&C_SceneObjectManager__RegisterSceneObject_original));

    MH_CreateHook(
        (LPVOID)SDK::gPatterns.C_SceneObjectManager__UnregisterSceneObject,
        (PBYTE)C_SceneObjectManager__UnregisterSceneObject,
        reinterpret_cast<void **>(&C_SceneObjectManager__UnregisterSceneObject_original));
}, "SceneObjectManager");
