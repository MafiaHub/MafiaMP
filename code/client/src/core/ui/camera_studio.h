#pragma once

#include "utils/safe_win32.h"

#include "sdk/ue/sys/core/c_scene_object.h"
#include "sdk/ue/sys/math/c_vector.h"

namespace MafiaMP::Core::UI {
    class CameraStudio final {
      private:
        bool _isEnabled                            = false;
        SDK::ue::sys::core::C_SceneObject *_camera = nullptr;
        SDK::ue::sys::math::C_Vector _camForward   = {1.0f, 0.0f, 0.0f};
        POINT _mouseDelta {};
        POINT _lastMousePos {};

      public:
        void Update();
    };
} // namespace MafiaMP::Core::UI
