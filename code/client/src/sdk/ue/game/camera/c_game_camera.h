#pragma once

#include <cstdint>
#include <vector>

#include "c_camera.h"

#include "sdk/ue/c_base_object.h"
#include "sdk/ue/dbg/renderutils/i_render_helper.h"

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

        class I_GameCamera {
          public:
            virtual ~I_GameCamera() = default;
        };

        class C_GameCamera : public I_GameCamera {
          public:
            std::vector<C_Camera*> m_vCameras;                          // 0008 - 002C
            float m_fUnk;                                               // 002C - 0030
            float m_fAspectRatio;                                       // 0030 - 0034
            float m_fUnk2;                                              // 0034 - 0038
            char pad0[0x8];                                             // 0038 - 0040
            C_BaseObject *m_pBaseObject;                                // 0040 - 0048
            void *m_pCameraEffectManager;                               // 0048 - 0050
            dbg::renderutils::I_RenderHelper *m_pRenderHelper;          // 0050 - 0058

          public:
            static C_GameCamera *GetInstanceInternal();
            virtual C_Camera *GetCamera(E_GameCameraID camID) = 0;
        };
    }
}
