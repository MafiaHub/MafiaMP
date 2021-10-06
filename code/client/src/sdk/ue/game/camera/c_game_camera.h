#pragma once

#include <cstdint>

#include "c_camera.h"

namespace SDK {
    namespace ue::game::camera {
        enum class E_GameCameraID : int32_t {
            CAMERA_UNK,
            CAMERA_PLAYER_MAIN_1,
            CAMERA_PLAYER_MAIN_2,
            CAMERA_PLAYER_MAIN_3,
            CAMERA_PLAYER_MAIN_4,
            CAMERA_PLAYER_SMALL_1,
            CAMERA_PLAYER_SMALL_2,
            CAMERA_PLAYER_SMALL_3,
            CAMERA_PLAYER_SMALL_4
        };

        class C_GameCamera {
          public:
            static C_GameCamera *GetInstanceInternal();
            virtual C_Camera *GetCamera(E_GameCameraID camID) = 0;
        };
    }
}
