#include "component_utils.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::sys::core {
        ue::C_Ptr<C_SceneObject> ComponentUtils::DuplicateSceneObject(const C_SceneObject *sourceObject, const char *newName) {
            ue::C_Ptr<C_SceneObject> result;
            hook::this_call<ue::C_Ptr<C_SceneObject> *>(gPatterns.ComponentUtils__DuplicateSceneObject, &result, sourceObject, newName);
            return result;
        }

        void ComponentUtils::Attach(const ue::C_Ptr<C_SceneObject> &parent, const ue::C_Ptr<C_SceneObject> &child, uint16_t slotId) {
            hook::this_call(gPatterns.ComponentUtils__Attach, &parent, &child, slotId);
        }

        void ComponentUtils::Detach(const ue::C_Ptr<C_SceneObject> &object) {
            // Detach takes the internal pointer (unk member), not a reference to C_Ptr
            hook::this_call(gPatterns.ComponentUtils__Detach, object.unk);
        }
    } // namespace ue::sys::core
} // namespace SDK
