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
    } // namespace ue::sys::core
} // namespace SDK
