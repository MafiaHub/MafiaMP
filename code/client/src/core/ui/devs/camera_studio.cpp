#include "camera_studio.h"

#include <algorithm>
#include <cmath>

#include "game/helpers/camera.h"
#include "game/helpers/controls.h"

#include "sdk/c_mafia_camera_module.h"
#include "sdk/mafia/framework/c_mafia_framework.h"
#include "sdk/mafia/framework/c_mafia_framework_interfaces.h"
#include "sdk/ue/game/camera/c_game_camera_mafia.h"
#include "sdk/ue/sys/core/c_scene_object.h"
#include "sdk/ue/sys/math/c_matrix.h"
#include "sdk/ue/sys/math/c_vector.h"

namespace {
    constexpr float kRadToDeg = 57.29577951f;
    constexpr float kDegToRad = 0.01745329252f;
    constexpr float kMaxPitch = 1.55f; // ~89 degrees

    SDK::ue::sys::math::C_Vector ForwardFromYawPitch(float yaw, float pitch) {
        const float cp = std::cos(pitch);
        return {std::sin(yaw) * cp, std::cos(yaw) * cp, std::sin(pitch)};
    }
} // namespace

namespace MafiaMP::Core::UI::Devs {
    SDK::ue::game::camera::C_GameCameraMafia *CameraStudio::GetGameCamera() const {
        const auto module = SDK::GetMafiaCameraModule();
        if (!module) {
            return nullptr;
        }
        return reinterpret_cast<SDK::ue::game::camera::C_GameCameraMafia *>(module->GetPlayerCamera());
    }

    void CameraStudio::SetStreamingAroundCamera(bool aroundCamera) {
        const auto framework = SDK::mafia::framework::C_MafiaFramework::GetInstance();
        if (!framework) {
            return;
        }

        const auto interfaces = framework->GetInterfaces();
        if (!interfaces) {
            return;
        }

        const auto streaming = interfaces->GetStreamingModule();
        if (!streaming) {
            return;
        }

        streaming->SetStreamingPosSource(aroundCamera ? SDK::mafia::streaming::E_StreamingPosSource::CAMERA : SDK::mafia::streaming::E_StreamingPosSource::PLAYER);
    }

    void CameraStudio::SeedFromCurrentCamera(SDK::ue::game::camera::C_GameCameraMafia *camera) {
        SDK::ue::sys::math::C_Vector pos = {};
        SDK::ue::sys::math::C_Vector dir = {};
        camera->GetPosDir(pos, dir);

        _position = pos;

        const float len = std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        if (len > 1e-4f) {
            dir.x /= len;
            dir.y /= len;
            dir.z /= len;
        }

        _yaw   = std::atan2(dir.x, dir.y);
        _pitch = std::asin(std::clamp(dir.z, -1.0f, 1.0f));
    }

    void CameraStudio::ApplyLocatorTransform() {
        if (!_locator) {
            return;
        }

        // Move the locator; the script-frame mode holds a weak pointer to it and reads its
        // transform every frame, so the camera follows. The game's own scripted cameras
        // drive the lock the same way: lock once, then move the bound object.
        auto transform = _locator->GetTransform();
        transform.SetPos(_position);
        transform.SetDir3(ForwardFromYawPitch(_yaw, _pitch));
        _locator->SetTransform(transform, SDK::ue::sys::core::E_TransformChangeType::DEFAULT);
    }

    void CameraStudio::EnableFreeCam(bool enable) {
        if (enable == _freeCamEnabled) {
            return;
        }

        const auto camera = GetGameCamera();
        if (!camera) {
            return;
        }

        if (enable) {
            if (!_locator) {
                _locator = SDK::ue::sys::core::C_SceneObject::CreateBlank();
            }
            if (!_locator) {
                return;
            }

            SeedFromCurrentCamera(camera);
            GetCursorPos(&_lastMousePos);

            // Place the locator, then lock the camera onto it once. From here we only move
            // the locator each frame.
            ApplyLocatorTransform();
            camera->LockTarget(_locator, nullptr, 0.0f);

            if (_lockControls) {
                MafiaMP::Game::Helpers::Controls::Lock(true);
            }
            if (_streamAtCamera) {
                SetStreamingAroundCamera(true);
            }
        }
        else {
            camera->Unlock();

            if (_lockControls) {
                MafiaMP::Game::Helpers::Controls::Lock(false);
            }
            if (_streamAtCamera) {
                SetStreamingAroundCamera(false);
            }
        }

        _freeCamEnabled = enable;
    }

    void CameraStudio::UpdateFreeCam(float deltaTime) {
        if (!GetGameCamera()) {
            return;
        }

        POINT mouse = {};
        GetCursorPos(&mouse);

        if (!_frozen) {
            if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0) {
                _yaw += static_cast<float>(mouse.x - _lastMousePos.x) * _sensitivity;
                _pitch -= static_cast<float>(mouse.y - _lastMousePos.y) * _sensitivity;
                _pitch = std::clamp(_pitch, -kMaxPitch, kMaxPitch);
            }

            const auto forward = ForwardFromYawPitch(_yaw, _pitch);
            const SDK::ue::sys::math::C_Vector right = {std::cos(_yaw), -std::sin(_yaw), 0.0f};

            float speed = _moveSpeed;
            if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                speed *= 3.0f;
            }
            if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                speed *= 0.25f;
            }
            const float step = speed * deltaTime;

            const auto move = [&](const SDK::ue::sys::math::C_Vector &v, float s) {
                _position.x += v.x * s;
                _position.y += v.y * s;
                _position.z += v.z * s;
            };

            if (GetAsyncKeyState('W') & 0x8000) {
                move(forward, step);
            }
            if (GetAsyncKeyState('S') & 0x8000) {
                move(forward, -step);
            }
            if (GetAsyncKeyState('D') & 0x8000) {
                move(right, step);
            }
            if (GetAsyncKeyState('A') & 0x8000) {
                move(right, -step);
            }
            if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                _position.z += step;
            }
            if (GetAsyncKeyState('C') & 0x8000) {
                _position.z -= step;
            }
        }

        // Anchor the mouse every frame (even when frozen) so unfreezing never jumps the view.
        _lastMousePos = mouse;

        ApplyLocatorTransform();
    }

    void CameraStudio::OnOpen() {}

    void CameraStudio::OnClose() {
        // Always restore the gameplay camera when the window is closed.
        if (_freeCamEnabled) {
            EnableFreeCam(false);
        }
    }

    void CameraStudio::OnUpdate() {
        const auto camera = GetGameCamera();

        auto windowContent = [&]() {
            if (!camera) {
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Game camera unavailable");
                return;
            }

            ImGui::TextColored(_freeCamEnabled ? ImVec4(0.3f, 1.0f, 0.3f, 1.0f) : ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Free camera: %s%s", _freeCamEnabled ? "ON" : "OFF",
                (_freeCamEnabled && _frozen) ? " (static)" : "");
            ImGui::TextDisabled("Hold RMB to look - WASD move - Space/C up/down - Shift fast - Ctrl slow");

            ImGui::Separator();

            if (ImGui::Button(_freeCamEnabled ? "Disable free camera" : "Enable free camera", ImVec2(-1.0f, 0.0f))) {
                EnableFreeCam(!_freeCamEnabled);
            }

            if (_freeCamEnabled) {
                ImGui::Checkbox("Freeze / detach (static camera)", &_frozen);

                ImGui::DragFloat3("Position", reinterpret_cast<float *>(&_position), 0.1f);

                float yawDeg   = _yaw * kRadToDeg;
                float pitchDeg = _pitch * kRadToDeg;
                if (ImGui::DragFloat("Yaw", &yawDeg, 0.5f, -180.0f, 180.0f)) {
                    _yaw = yawDeg * kDegToRad;
                }
                if (ImGui::DragFloat("Pitch", &pitchDeg, 0.5f, -89.0f, 89.0f)) {
                    _pitch = pitchDeg * kDegToRad;
                }

                ImGui::Separator();
                ImGui::SliderFloat("Move speed", &_moveSpeed, 1.0f, 200.0f, "%.0f u/s");
                ImGui::SliderFloat("Mouse sensitivity", &_sensitivity, 0.0005f, 0.01f, "%.4f");
                ImGui::Checkbox("Lock player controls", &_lockControls);
                ImGui::SameLine();
                ImGui::Checkbox("Stream around camera", &_streamAtCamera);
            }

            ImGui::Separator();
            ImGui::TextDisabled("Gameplay camera");
            if (ImGui::Button("Toggle FPV")) {
                static bool fpv = false;
                fpv             = !fpv;
                MafiaMP::Game::Helpers::Camera::SetFPV(fpv);
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset behind player")) {
                MafiaMP::Game::Helpers::Camera::ResetBehindPlayer();
            }
        };

        CreateUIWindow("Camera Studio", windowContent, &_open);

        if (_freeCamEnabled) {
            float dt = ImGui::GetIO().DeltaTime;
            if (dt <= 0.0f) {
                dt = 1.0f / 60.0f;
            }
            UpdateFreeCam(dt);
        }
    }
} // namespace MafiaMP::Core::UI::Devs
