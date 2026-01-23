#pragma once

#include "c_component_object.h"
#include "i_component.h"
#include "../math/c_matrix.h"
#include "../../c_string.h"

#include <cstdint>

namespace SDK {
    namespace ue::sys::core {
        class C_Scene;

        enum class E_TransformChangeType : int {
            DEFAULT   = 0,
            NO_NOTIFY = 2
        };

        enum class E_SceneObjectOrigin : int {
            UNKNOWN    = 0,
            STREAMING  = 1,
            DUPLICATED = 2,
            PROCEDURAL = 3
        };

        class C_SceneObject : public C_ComponentObject {
          public:
            uint64_t GetNameHash() const {
                return *reinterpret_cast<const uint64_t *>(reinterpret_cast<const char *>(this) + 0x48);
            }

            const char *GetName() const {
                return *reinterpret_cast<const char *const *>(reinterpret_cast<const char *>(this) + 0x50);
            }

            bool IsActivated() const {
                return (m_wFlags & 0x01) != 0;
            }

            C_Scene *GetScene() const {
                return m_pScene;
            }

            const ue::sys::math::C_Matrix &GetTransformRef() const {
                return m_mTransform;
            }

            ue::sys::math::C_Matrix GetTransform();
            void SetTransform(const ue::sys::math::C_Matrix &transform, E_TransformChangeType changeType);

          protected:
            C_Scene *m_pScene;                        // 0x58 - 0x60
            ue::sys::math::C_Matrix m_mTransform;     // 0x60 - 0x90
            uint32_t m_dwUnk90;                       // 0x90 - 0x94
            uint16_t m_wFlags;                        // 0x94 - 0x96
            uint16_t m_wCounter;                      // 0x96 - 0x98
            void *m_pUnk98;                           // 0x98 - 0xA0
        };
    }; // namespace ue::sys::core
} // namespace SDK
