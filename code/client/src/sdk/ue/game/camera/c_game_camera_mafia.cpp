#include "c_game_camera_mafia.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::camera {
        // 4th arg is a target-space offset vector that SetParameters dereferences
        // unconditionally (a null pointer faults), and blendTime is a float in the 5th slot,
        // so call through a typed pointer to keep the ABI/register marshalling correct.
        using LockTarget_t = void(__fastcall *)(C_GameCameraMafia *, ue::sys::core::C_SceneObject *, ue::sys::core::C_SceneObject *, const ue::sys::math::C_Vector *, float);

        void C_GameCameraMafia::LockTarget(ue::sys::core::C_SceneObject *posObject, ue::sys::core::C_SceneObject *targetObject, float blendTime) {
            const ue::sys::math::C_Vector offset {}; // zero offset
            reinterpret_cast<LockTarget_t>(gPatterns.C_GameCameraMafia__LockTarget)(this, posObject, targetObject, &offset, blendTime);
        }

        void C_GameCameraMafia::Unlock() {
            ModePopImmediate(false, E_GameCameraLayer::LAYER_SCRIPT, false);
        }
    } // namespace ue::game::camera
} // namespace SDK
