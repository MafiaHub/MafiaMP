#pragma once

#include <cstdint>

#include "c_ticked_module.h"
#include "sdk/ue/game/camera/c_camera.h"
#include "sdk/ue/game/camera/c_car_back_mirror.h"
#include "sdk/ue/game/camera/c_evaluator_factory.h"
#include "sdk/ue/game/camera/c_game_camera_mafia.h"

namespace SDK {
    class C_MafiaCameraModule : public C_TickedModule {
      public:
        ue::game::camera::C_EvaluatorFactory *m_pEvaluatorFactory;  // 0008 - 0010
        int32_t m_iState;                                           // 0010 - 0014
        int32_t _pad14;                                             // 0014 - 0018
        void *m_pUnknownPtr;                                        // 0018 - 0020
        ue::game::camera::C_GameCameraMafia *m_pCamera;             // 0020 - 0028
        bool m_bInitialized;                                        // 0028 - 0029
        bool m_bPaused;                                             // 0029 - 002A
        uint16_t _pad2A;                                            // 002A - 002C
        int32_t m_iGUIId;                                           // 002C - 0030
        ue::game::camera::C_CarBackMirror *m_pCarBackMirror;        // 0030 - 0038

      public:
        ue::game::camera::C_Camera *GetPlayerCamera();
    };

    // Static size: 0x38 (56 bytes)
    static_assert(sizeof(C_MafiaCameraModule) == 0x38, "C_MafiaCameraModule size mismatch");

    C_MafiaCameraModule *GetMafiaCameraModule();
} // namespace SDK
