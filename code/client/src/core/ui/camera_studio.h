#pragma once

#include "utils/safe_win32.h"

#include "sdk/ue/sys/core/c_scene_object.h"
#include "sdk/ue/sys/math/c_vector.h"

namespace MafiaMP::Core::UI {
    class CameraStudio final {
      public:
        CameraStudio();

        void Update();

        bool IsVisible() const {
            return _visible;
        }

        void SetVisible(bool visible) {
            _visible = visible;
        }

      private:
        bool _visible = false;

        SDK::ue::sys::core::C_SceneObject *_camera = nullptr;

        SDK::ue::sys::math::C_Vector _camForward = {1.0f, 0.0f, 0.0f};

        POINT _mouseDelta {};

        POINT _lastMousePos {};
    };
} // namespace MafiaMP::Core::UI
