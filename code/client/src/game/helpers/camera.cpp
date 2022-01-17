#include "camera.h"

#include "../../sdk/ue/game/camera/c_game_camera.h"

namespace MafiaMP::Game::Helpers {
    bool Camera::SetPos(SDK::ue::sys::math::C_Vector start, SDK::ue::sys::math::C_Vector end, bool interpolate) {
        const auto gameCamera = SDK::ue::game::camera::C_GameCamera::GetInstanceInternal();
        if (!gameCamera) {
            return false;
        }

        const auto camera = gameCamera->GetCamera(SDK::ue::game::camera::E_GameCameraID::CAMERA_PLAYER_MAIN_1);
        if (!camera) {
            return false;
        }


        return true;
    }

    bool Camera::ResetBehindPlayer() {
        return true;
    }

    bool Camera::SimpleShake() {
        return true;
    }
}
