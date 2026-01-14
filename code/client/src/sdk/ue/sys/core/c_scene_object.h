#pragma once

#include "../math/c_matrix.h"
#include "../../c_string.h"

#include <cstdint>

namespace SDK {
    namespace ue::sys::core {
        enum class E_TransformChangeType { DEFAULT };

        class C_SceneObject {
          public:
            uint64_t GetNameHash() const {
                return *reinterpret_cast<const uint64_t *>(reinterpret_cast<const char *>(this) + 0x48);
            }

            C_String *GetName() {
                return reinterpret_cast<C_String *>(reinterpret_cast<char *>(this) + 0x50);
            }

            ue::sys::math::C_Matrix GetTransform();
            void SetTransform(const ue::sys::math::C_Matrix &transform, E_TransformChangeType changeType);
        };
    }; // namespace ue::sys::core
} // namespace SDK
