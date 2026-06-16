#pragma once

#include "utils/safe_win32.h"

#include "../ui_base.h"

#include "sdk/ue/sys/core/c_scene_object.h"
#include "sdk/ue/sys/math/c_vector.h"

namespace SDK::ue::game::camera {
    class C_GameCameraMafia;
}

namespace MafiaMP::Core::UI::Devs {
    // Free camera: detaches the game camera onto a hidden locator scene object and flies it
    // with WASD + mouse-look (noclip), or freezes it in place (static detached camera).
    class CameraStudio final: public UIBase {
      private:
        SDK::ue::sys::core::C_SceneObject *_locator = nullptr;

        bool _freeCamEnabled = false;
        bool _frozen         = false; // ignore movement input (static camera)
        bool _lockControls   = true;
        bool _streamAtCamera = true;

        // Z-up, yaw about Z.
        SDK::ue::sys::math::C_Vector _position {};
        float _yaw         = 0.0f;
        float _pitch       = 0.0f;
        float _moveSpeed   = 12.0f;
        float _sensitivity = 0.0035f;

        POINT _lastMousePos {};

        SDK::ue::game::camera::C_GameCameraMafia *GetGameCamera() const;

        void EnableFreeCam(bool enable);
        void SeedFromCurrentCamera(SDK::ue::game::camera::C_GameCameraMafia *camera);
        void UpdateFreeCam(float deltaTime);
        void ApplyLocatorTransform();
        void SetStreamingAroundCamera(bool aroundCamera);

        virtual void OnOpen() override;

        virtual void OnClose() override;

        virtual void OnUpdate() override;

      public:
        CameraStudio(): UIBase() {};
    };
} // namespace MafiaMP::Core::UI::Devs
