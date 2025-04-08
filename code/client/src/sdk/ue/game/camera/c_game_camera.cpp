#include "c_game_camera.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::camera {
        C_GameCamera *C_GameCamera::GetInstanceInternal() {
            return hook::call<C_GameCamera *>(gPatterns.C_GameCamera__GetInstanceInternal);
        }

        C_Camera *C_GameCamera::GetCamera(E_GameCameraID camID) {
            return hook::this_call<C_Camera *>(gPatterns.C_GameCamera__GetCamera, this, camID);
        }
    } // namespace ue::game::camera
} // namespace SDK
