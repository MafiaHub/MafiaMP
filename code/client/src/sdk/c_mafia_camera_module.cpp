#include "c_mafia_camera_module.h"

#include "patterns.h"

namespace SDK {
    ue::game::camera::C_Camera* C_MafiaCameraModule::GetPlayerCamera() {
        return hook::this_call<ue::game::camera::C_Camera *>(gPatterns.C_MafiaCameraModule__GetPlayerCamera, this);
    }

    C_MafiaCameraModule *GetMafiaCameraModule() {
        return hook::call<C_MafiaCameraModule *>(gPatterns.C_MafiaCameraModule__GetMafiaCameraModule);
    }
}
