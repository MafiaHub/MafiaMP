#pragma once

#include "c_component_object.h"
#include "i_component.h"
#include "../math/c_matrix.h"
#include "../../c_string.h"

#include <cstdint>

namespace SDK {
    namespace ue::sys::core {
        enum class E_TransformChangeType { DEFAULT };

        /**
         * C_SceneObject - Scene graph object representation
         * Size: at least 0xA0 (160 bytes)
         *
         * Reversed from:
         * - Constructor: 0x143295e60
         * - Activate: 0x1432bd430
         * - Vtable: 0x144F04008
         *
         * Inherits from C_ComponentObject which provides component-based architecture
         * and adds scene graph specific functionality like transform matrix.
         */
        class C_SceneObject : public C_ComponentObject {
          public:
            uint64_t GetNameHash() const {
                return *reinterpret_cast<const uint64_t *>(reinterpret_cast<const char *>(this) + 0x48);
            }

            const char *GetName() const {
                return *reinterpret_cast<const char *const *>(reinterpret_cast<const char *>(this) + 0x50);
            }

            ue::sys::math::C_Matrix GetTransform();
            void SetTransform(const ue::sys::math::C_Matrix &transform, E_TransformChangeType changeType);

          protected:
            void *m_pUnk58;                           // 0x58 - 0x60
            ue::sys::math::C_Matrix m_mTransform;     // 0x60 - 0x90
            uint32_t m_dwUnk90;                       // 0x90 - 0x94
            uint16_t m_wFlags;                        // 0x94 - 0x96
            uint16_t m_wCounter;                      // 0x96 - 0x98
            void *m_pUnk98;                           // 0x98 - 0xA0
        };
    }; // namespace ue::sys::core
} // namespace SDK
