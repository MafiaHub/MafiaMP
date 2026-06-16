#pragma once

#include <cstdint>

#include "c_camera.h"
#include "s_generic_sync_object_data.h"

#include "sdk/ue/sys/core/c_scene_object.h"

namespace SDK {
    namespace ue::game::camera {

        /**
         * C_GameCameraMafia - Mafia-specific game camera implementation
         *
         * Inherits from C_Camera (base size 0x328 / 808 bytes)
         * Total size: 0x340 (832 bytes)
         *
         * Additional members on top of C_Camera:
         * - 0x328: m_pSyncObjectData1 (S_GenericSyncObjectData*)
         * - 0x330: m_pSyncObjectData2 (S_GenericSyncObjectData*)
         * - 0x338: m_pSyncObjectData3 (S_GenericSyncObjectData*)
         */
        class C_GameCameraMafia : public C_Camera {
          public:
            S_GenericSyncObjectData *m_pSyncObjectData1;                    // 0328 - 0330
            S_GenericSyncObjectData *m_pSyncObjectData2;                    // 0330 - 0338
            S_GenericSyncObjectData *m_pSyncObjectData3;                    // 0338 - 0340

          public:
            // Detaches the camera onto posObject (its forward axis becomes the view
            // direction, or targetObject when given). blendTime <= 0 snaps. Pushes
            // C_CM_ScriptFrame on LAYER_SCRIPT; undo with Unlock(). Objects must outlive the lock.
            void LockTarget(ue::sys::core::C_SceneObject *posObject, ue::sys::core::C_SceneObject *targetObject = nullptr, float blendTime = 0.0f);
            void Unlock();

            void ScriptForcedFOVStart(float fov, bool immediate);
            void ScriptLookAtDir(sys::math::C_Vector *dir, sys::math::C_Vector *offset, sys::math::C_Vector *targetOffset, float transitionTime, float holdTime, float returnTime, float fov, bool smooth, bool useOffset, bool lockRotation);
            void ScriptLookAtStop();
            void ScriptPointAtVec(sys::math::C_Vector *destVector, float transitionTime, float rotationEnabled, int lockedTime);
            void ScriptResetCamBehindPlayer();
            void SetImmediateCamera(bool immediate);
            void SetShakeOverrideEnabled(bool enabled);
            void SetShakeOverrideSineWaveEasyParams(sys::math::C_Vector *posAmplitude, sys::math::C_Vector *rotAmplitude, sys::math::C_Vector *targetAmplitude, float frequency, float duration);
        };

        // Static size: 0x340 (832 bytes)
        static_assert(sizeof(C_GameCameraMafia) == 0x340, "C_GameCameraMafia size mismatch");
    } // namespace ue::game::camera
} // namespace SDK
