#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::game::crashobject {
        /**
         * Base class for deformable physics objects.
         * Used by C_Vehicle and other crashable/deformable entities.
         * Size: 0x248 (584 bytes)
         */
        class C_Deformation {
          public:
            void ResetDynamics(int partIndex);

          protected:
            char pad0[0x38];                    // 0000 - 0038
            void **m_pSceneObjectsStart;        // 0038 - 0040 (array of scene objects)
            void **m_pSceneObjectsEnd;          // 0040 - 0048
            void **m_pSceneObjectsCapacity;     // 0048 - 0050
            char pad1[0x28];                    // 0050 - 0078
            int32_t m_iFlags;                   // 0078 - 007C
            float m_fDefaultFloat;              // 007C - 0080 (initialized to 1.0f)
            char pad2[0x28];                    // 0080 - 00A8
            char m_deformationData[0x180];      // 00A8 - 0228 (embedded deformation sub-structure)
            char pad3[0x20];                    // 0228 - 0248
        };

        static_assert(sizeof(C_Deformation) == 0x248, "C_Deformation size mismatch");
    } // namespace ue::game::crashobject
} // namespace SDK
