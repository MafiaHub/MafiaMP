#pragma once

#include "../math/c_matrix.h"
#include "../c_string.h"

namespace SDK {
    namespace ue::sys::core {
        enum class E_TransformChangeType { DEFAULT };

        class C_SceneObject {
          public:
            C_String *GetName() {
                return (C_String *)(this + 0x50);
            }

            ue::sys::math::C_Matrix GetTransform();
            void SetTransform(const ue::sys::math::C_Matrix &transform, E_TransformChangeType changeType);
        };
    }; // namespace ue::sys::core
} // namespace SDK