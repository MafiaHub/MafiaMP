#include "c_game_camera.h"

#include <utils/hooking/hooking.h>
#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::camera {
        C_GameCamera *C_GameCamera::GetInstanceInternal() {
            return hook::call<C_GameCamera *>(gPatterns.C_GameCamera__GetInstanceInternalAddr);
        }
    }
}
