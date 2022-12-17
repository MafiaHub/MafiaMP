#include "camera_studio.h"
#include "game/helpers/controls.h"

#include "sdk/entities/c_actor.h"
#include "sdk/mafia/framework/c_mafia_framework.h"
#include "sdk/mafia/framework/c_mafia_framework_interfaces.h"

#include <imgui/imgui.h>

#include <MinHook.h>
#include <utils/hooking/hooking.h>

// todo move to hooks
typedef void(__fastcall *C_GameCameraMafia__LockTarget_t)(void *_this, void *targetSceneObject, float distance);
C_GameCameraMafia__LockTarget_t C_GameCameraMafia__LockTarget_Original = nullptr;
void *C_GameCameraPtr                                                  = nullptr;

void __fastcall C_GameCameraMafia__LockTarget(void *_this, void *targetSceneObject, float distance) {
    C_GameCameraPtr = _this;
    C_GameCameraMafia__LockTarget_Original(_this, targetSceneObject, distance);
}

static POINT GetMouseXY(VOID) {
    POINT newPos = {0};
    GetCursorPos(&newPos);
    return newPos;
}

namespace MafiaMP::Core::UI {
    CameraStudio::CameraStudio() {
        // todo find a pattern
        // MH_CreateHook((LPVOID)0x0000000142DE3870, (PBYTE)C_GameCameraMafia__LockTarget, reinterpret_cast<void **>(&C_GameCameraMafia__LockTarget_Original));
        // MH_EnableHook(MH_ALL_HOOKS);
    }

    void CameraStudio::Update() {
        ImGui::Begin("Camera studio");
        {
            if (ImGui::Button("Free fly")) {
                /*void *allocatedMemory = hook::this_call<void *>(0x000000014384FAD0, 0xA0);
                _camera               = hook::call<SDK::ue::sys::core::C_SceneObject *>(0x0000000143293180, allocatedMemory);

                // Allocate refCounter
                uint16_t *refCounter = reinterpret_cast<uint16_t *>(hook::this_call<uint64_t>(0x140994930, (uint64_t)_camera));

                // Set scene object ref counter
                *(uint64_t *)(_camera + 0x58) = (uint64_t)refCounter;

                auto activePlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();

                if (activePlayer) {
                    auto playerActor = reinterpret_cast<SDK::C_Actor *>(activePlayer);
                    auto playerPos   = playerActor->GetPos();

                    auto currentTransform = _camera->GetTransform();
                    currentTransform.SetPos(playerPos);
                    _camera->SetTransform(currentTransform, SDK::ue::sys::core::E_TransformChangeType::DEFAULT);
                }

                C_GameCameraMafia__LockTarget_Original(C_GameCameraPtr, _camera, 0.0f);*/
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
            C_GameCameraMafia__LockTarget_Original(C_GameCameraPtr, _camera, 0.0f);
        }
    }
} // namespace MafiaMP::Core::UI
