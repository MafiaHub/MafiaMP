#include "camera_studio.h"

#include <MinHook.h>
#include <utils/hooking/hooking.h>

#include "game/helpers/controls.h"

#include "sdk/entities/c_actor.h"
#include "sdk/c_mafia_camera_module.h"
#include "sdk/ue/game/camera/c_game_camera.h"
#include "sdk/mafia/framework/c_mafia_framework.h"
#include "sdk/mafia/framework/c_mafia_framework_interfaces.h"

// todo move to hooks
typedef void(__fastcall *C_GameCameraMafia__LockTarget_t)(void *_this, void *targetSceneObject, void *targetSceneObject2, void *unk, float distance);
C_GameCameraMafia__LockTarget_t C_GameCameraMafia__LockTarget_Original = nullptr;
void *C_GameCameraPtr                                                  = nullptr;

void __fastcall C_GameCameraMafia__LockTarget(void *_this, void *targetSceneObject, void *targetSceneObject2, void *unk, float distance) {
    C_GameCameraPtr = _this;
    C_GameCameraMafia__LockTarget_Original(_this, targetSceneObject, targetSceneObject2, unk, distance);
}

static POINT GetMouseXY(VOID) {
    POINT newPos = {0};
    GetCursorPos(&newPos);
    return newPos;
}

static InitFunction init([]() {
    auto C_GameCameraMafia__LockTargetAddr = reinterpret_cast<uint64_t>(hook::pattern("48 85 D2 0F 84 ? ? ? ? 48 8B C4 48 89 58 18").get_first());
    MH_CreateHook((LPVOID)C_GameCameraMafia__LockTargetAddr, (PBYTE)C_GameCameraMafia__LockTarget, reinterpret_cast<void **>(&C_GameCameraMafia__LockTarget_Original));
    },
    "CameraStudio");

namespace MafiaMP::Core::UI::Devs {
    void CameraStudio::OnOpen() {}

    void CameraStudio::OnClose() {}

    void CameraStudio::OnUpdate() {
        const auto pGameCamera    = SDK::ue::game::camera::C_GameCamera::GetInstanceInternal();
        if (!pGameCamera) {
            return;
        }

        const auto pCameraModule = SDK::GetMafiaCameraModule();
        if (!pCameraModule) {
            return;
        }

        const auto pPlayerCamera = pCameraModule->GetPlayerCamera();
        if (!pPlayerCamera) {
            return;
        }

        SDK::ue::sys::math::C_Vector pos = {};
        SDK::ue::sys::math::C_Vector dir = {};
        SDK::ue::sys::math::C_Vector vec3 = {};

        pPlayerCamera->GetPosDir(pos, dir);

        auto windowContent = [&]() {
            {
                ImGui::Text("GameCamera Ptr : 0x%p\n", pGameCamera);
                ImGui::Text("GameCamera BaseObject Ptr: 0x%p\n", pGameCamera->m_pBaseObject);
                ImGui::Text("GameCamera RenderHelper Ptr: 0x%p\n", pGameCamera->m_pRenderHelper);
                ImGui::Text("GameCamera CameraEffectManager Ptr: 0x%p\n", pGameCamera->m_pCameraEffectManager);
                ImGui::Text("Aspect Ratio: %f\n", pGameCamera->m_fAspectRatio);
                ImGui::Text("PlayerCamera Cameras Count: %i\n", pGameCamera->m_vCameras.size());
            }

            ImGui::Separator();

            {
                ImGui::Text("PlayerCamera Ptr : 0x%p\n", pPlayerCamera);
                ImGui::Text("PlayerCamera IsEnabled = %s\n", pPlayerCamera->IsEnabled() ? "true" : "false");
                ImGui::Text("PlayerCamera Mode Active Type = %d (%s)\n", pPlayerCamera->ModeGetActiveTypeTop(), SDK::ue::game::camera::GetGameCameraModeString(pPlayerCamera->ModeGetActiveTypeTop()));
                ImGui::Text("PlayerCamera Object : %s\n", pPlayerCamera->m_sCameraObject);

                if (ImGui::DragFloat3("Position", (float *)&pos, 0.1f, -4500.0f, 4500.0f)) {}

                if (ImGui::DragFloat3("Direction", (float *)&dir, 0.1f, -4500.0f, 4500.0f)) {}
            }

            ImGui::Separator();

            {
                if (ImGui::Button("Toggle")) {
                    pPlayerCamera->Enable(!pPlayerCamera->IsEnabled());
                }
                ImGui::SameLine();
                if (ImGui::Button("Reset Behind Player")) {
                    pPlayerCamera->ModePopImmediate(true, SDK::ue::game::camera::E_GameCameraLayer::LAYER_ACTOR, true);
                }
                ImGui::SameLine();
                if (ImGui::Button("FPV Mode")) {
                    pPlayerCamera->ModeChangeImmediate(SDK::ue::game::camera::E_GameCameraModeID::E_GCM_FPV, nullptr, true, true, true);
                }
            }


        };

        CreateUIWindow("Camera Studio", windowContent, &_open);

        /* ImGui::Begin("Camera studio", &_open);
        {
            if (ImGui::Button("Enable")) {
                auto addr1            = hook::get_opcode_address("E8 ? ? ? ? 33 F6 EB 9F");
                auto addr2            = hook::get_opcode_address("E8 ? ? ? ? 4C 8B E8 49 8B CD");
                auto addr3            = hook::get_opcode_address("E8 ? ? ? ? 49 8B 4C 3F ?");
                void *allocatedMemory = hook::this_call<void *>(addr1, 0xA0);
                _camera               = hook::call<SDK::ue::sys::core::C_SceneObject *>(addr2, allocatedMemory);

                // Allocate refCounter
                uint16_t *refCounter = reinterpret_cast<uint16_t *>(hook::this_call<uint64_t>(addr3, (uint64_t)_camera));

                // // Set scene object ref counter
                *(uint64_t *)(_camera + 0x58) = (uint64_t)refCounter;

                auto activePlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();

                if (activePlayer) {
                    auto playerActor = reinterpret_cast<SDK::C_Actor *>(activePlayer);
                    auto playerPos   = playerActor->GetPos();

                    auto currentTransform = _camera->GetTransform();
                    currentTransform.SetPos(playerPos);
                    _camera->SetTransform(currentTransform, SDK::ue::sys::core::E_TransformChangeType::DEFAULT);
                }

                C_GameCameraMafia__LockTarget_Original(C_GameCameraPtr, _camera, 0, 0, 0.0f);
            }
        }
        ImGui::End();

        if (_camera) {
            auto upVector = SDK::ue::sys::math::C_Vector(0.0f, 0.0f, 1.0f);

            POINT mousePos = GetMouseXY();
            _mouseDelta.x  = -(_lastMousePos.x - mousePos.x);
            _mouseDelta.y  = -(_lastMousePos.y - mousePos.y);
            _lastMousePos  = mousePos;

            if ((GetKeyState(VK_RBUTTON) & 0x100) != 0) {
                if (abs(_mouseDelta.x) > 0) {
                    _camForward = _camForward.applyAxisAngle(upVector, -_mouseDelta.x * 0.005f);
                }

                if (abs(_mouseDelta.y) > 0) {
                    _camForward = _camForward.applyAxisAngle(upVector.cross(_camForward).normalized(), _mouseDelta.y * 0.005f);
                }
            }

            auto currentTransform = _camera->GetTransform();
            currentTransform.SetDir3(_camForward);

            auto currentPos  = currentTransform.GetPos();
            auto rightVector = _camForward.cross(upVector);

            if (GetAsyncKeyState(0x41 + 'w' - 'a')) {
                currentPos = currentPos + _camForward;
            }

            if (GetAsyncKeyState(0x41 + 's' - 'a')) {
                currentPos = currentPos - _camForward;
            }

            if (GetAsyncKeyState(0x41 + 'd' - 'a')) {
                currentPos = currentPos + rightVector;
            }

            if (GetAsyncKeyState(0x41 + 'a' - 'a')) {
                currentPos = currentPos - rightVector;
            }

            if (GetAsyncKeyState(VK_SPACE)) {
                currentPos.z += 0.2f;
            }

            if (GetAsyncKeyState(VK_SHIFT)) {
                currentPos.z -= 0.2f;
            }

            currentTransform.SetPos(currentPos);
            _camera->SetTransform(currentTransform, SDK::ue::sys::core::E_TransformChangeType::DEFAULT);
            auto streamingModule = SDK::mafia::framework::C_MafiaFramework::GetInstance()->GetInterfaces()->GetStreamingModule();
            if (streamingModule) {
                auto result = streamingModule->RequestLoadAreasInRadius(currentPos, 100.0f, 8);
            }
            C_GameCameraMafia__LockTarget_Original(C_GameCameraPtr, _camera, 0, 0, 0.0f);
        }*/
    }
} // namespace MafiaMP::Core::UI::Devs
