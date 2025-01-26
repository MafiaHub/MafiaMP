#pragma once

#include "c_ticked_module.h"
#include "sdk/ue/game/camera/c_game_camera.h"

namespace SDK {
    class C_MafiaCameraModule : public C_TickedModule {
      public:
        ue::game::camera::C_GameCamera *GetPlayerCamera();
    };

    C_MafiaCameraModule *GetMafiaCameraModule();
}
