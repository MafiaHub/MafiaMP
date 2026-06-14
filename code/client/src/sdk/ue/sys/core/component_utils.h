#pragma once

#include "c_scene_object.h"
#include "../../c_ptr.h"

#include <cstdint>

namespace SDK {
    namespace ue::sys::core {
        class ComponentUtils {
          public:
            // Duplicates a scene object with a new name
            // Returns a C_Ptr to the duplicated object
            static ue::C_Ptr<C_SceneObject> DuplicateSceneObject(const C_SceneObject *sourceObject, const char *newName);

            // Attaches a child scene object to a parent scene object at a specific slot
            static void Attach(const ue::C_Ptr<C_SceneObject> &parent, const ue::C_Ptr<C_SceneObject> &child, uint16_t slotId);

            // Detaches a scene object from its parent
            static void Detach(const ue::C_Ptr<C_SceneObject> &object);
        };
    }; // namespace ue::sys::core
} // namespace SDK
