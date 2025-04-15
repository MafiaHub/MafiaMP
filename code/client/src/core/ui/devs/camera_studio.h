#pragma once

#include "utils/safe_win32.h"

#include "../ui_base.h"

#include "sdk/ue/sys/core/c_scene_object.h"
#include "sdk/ue/sys/math/c_vector.h"

namespace MafiaMP::Core::UI::Devs {
    class CameraStudio final: public UIBase {
      private:
        SDK::ue::sys::core::C_SceneObject *_camera = nullptr;

        SDK::ue::sys::math::C_Vector _camForward = {1.0f, 0.0f, 0.0f};

        POINT _mouseDelta {};

        POINT _lastMousePos {};

        virtual void OnOpen() override;

        virtual void OnClose() override;

        virtual void OnUpdate() override;

      public:
        CameraStudio(): UIBase() {};
    };
} // namespace MafiaMP::Core::UI::Devs
