#include "camera.h"

#include <fmt/format.h>

#include "../../core/application.h"

#include "controls.h"
#include "../../sdk/ue/game/camera/c_game_camera.h"

namespace MafiaMP::Game::Helpers {
    bool Camera::SetFPV(bool toggle) {
        const auto gameCamera = SDK::ue::game::camera::C_GameCamera::GetInstanceInternal();
        if (!gameCamera) {
            return false;
        }

        const auto camera = gameCamera->GetCamera(SDK::ue::game::camera::E_GameCameraID::CAMERA_PLAYER_MAIN_1);
        if (!camera) {
            return false;
        }

        if (toggle) {
            camera->ModeChangeImmediate(SDK::ue::game::camera::E_GameCameraModeID::E_GCM_FPV, nullptr, true, true, true);
        }
        else {
            if (camera->ModeGetActiveTypeTop() == SDK::ue::game::camera::E_GameCameraModeID::E_GCM_FPV) {
                camera->ModePopImmediate(true, SDK::ue::game::camera::E_GameCameraLayer::LAYER_ACTOR, true);
            }
        }

        return true;
    }

    bool Camera::SetPos(SDK::ue::sys::math::C_Vector start, SDK::ue::sys::math::C_Vector end, bool interpolate) {
        SDK::ue::sys::math::C_Vector finalVec = {start.x + end.x, start.y + end.y, start.z + end.z};
        const auto formattedVector            = fmt::format("Math:newVector({}, {}, {})", finalVec.x, finalVec.y, finalVec.z);
        const auto command                    = fmt::format("game.cameramanager:GetPlayerCamera():ScriptPointAtVec({}, 1, 1, 1)", formattedVector.c_str());
        return Core::gApplication->GetLuaVM()->ExecuteString(command.c_str());
    }

    bool Camera::ResetBehindPlayer() {
        const auto gameCamera = SDK::ue::game::camera::C_GameCamera::GetInstanceInternal();
        if (!gameCamera) {
            return false;
        }

        const auto camera = gameCamera->GetCamera(SDK::ue::game::camera::E_GameCameraID::CAMERA_PLAYER_MAIN_1);
        if (!camera) {
            return false;
        }

        camera->ModePopImmediate(true, SDK::ue::game::camera::E_GameCameraLayer::LAYER_ACTOR, true);

        return true;
    }

    bool Camera::SimpleShake() {
        return true;
    }
}
