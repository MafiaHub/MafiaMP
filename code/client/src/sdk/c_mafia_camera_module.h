#pragma once

#include "c_ticked_module.h"
#include "sdk/ue/game/camera/c_camera.h"
#include "sdk/ue/game/camera/c_car_back_mirror.h"

namespace SDK {
    class C_MafiaCameraModule : public C_TickedModule {
      public:
        char pad0[0x28];                                            // 0008 - 0030
        ue::game::camera::C_CarBackMirror *m_pCarBackMirror;        // 0030 - 0038

      public:
        ue::game::camera::C_Camera *GetPlayerCamera();
    };

    C_MafiaCameraModule *GetMafiaCameraModule();
}
