#include "c_scene_object.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::sys::core {
        ue::sys::math::C_Matrix C_SceneObject::GetTransform() {
            return m_mTransform;
        }

        void C_SceneObject::SetTransform(const ue::sys::math::C_Matrix &transform, E_TransformChangeType changeType) {
            hook::this_call(gPatterns.C_SceneObject__SetTransform, this, transform, changeType);
        }

        C_SceneObject *C_SceneObject::CreateBlank() {
            void *memory = hook::call<void *>(gPatterns.ue__OperatorNew, 0xA0ull); // sizeof(C_SceneObject)
            if (!memory) {
                return nullptr;
            }

            auto *object = hook::call<C_SceneObject *>(gPatterns.C_SceneObject__C_SceneObject, memory);

            // +0x58 holds the object's ref-count control; weak pointers (e.g. the camera's
            // LockTarget) read it, so wire it up before handing the object out.
            void *refControl = hook::call<void *>(gPatterns.C_SceneObject__AllocRefControl, object);
            *reinterpret_cast<void **>(reinterpret_cast<uintptr_t>(object) + 0x58) = refControl;

            return object;
        }
    } // namespace ue::sys::core
} // namespace SDK
