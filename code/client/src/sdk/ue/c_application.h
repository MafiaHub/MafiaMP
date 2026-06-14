#pragma once

#include "sdk/c_game_time.h"
#include "sdk/ue/c_game_fsm.h"
#include "sdk/ue/i_engine.h"
#include "sdk/ue/sys/utils/c_port_cullis.h"

namespace SDK {
    namespace ue {
        class C_Application {
          public:
            char pad[0x10];                              // 0000 - 0010 (vtables)
            C_CommandLine *m_pCommandLine;               // 0010 - 0018
            uint32_t m_nMinFrameTimeMs;                  // 0018 - 001C (default 33ms, ~30 FPS)
            uint32_t m_nMaxFrameTimeMs;                  // 001C - 0020 (default 33ms)
            float m_fConstantFpsInterval;                // 0020 - 0024 (time between frames for constant FPS mode)
            uint32_t m_nFrameWaitTimeMs;                 // 0024 - 0028 (default 49ms, ~20 FPS minimum)
            char pad0[0x4];                              // 0028 - 002C
            uint32_t m_iLastFrameTime;                   // 002C - 0030
            uint32_t m_iState;                           // 0030 - 0034 (0=quit, 1=shutdown, 2=running)
            char pad1[0x4];                              // 0034 - 0038
            void *m_pTimerList;                          // 0038 - 0040 (internal timing structure)
            char pad2[0x8];                              // 0040 - 0048
            uint32_t m_iDebugTickCounter;                // 0048 - 004C
            uint32_t m_nRefCount;                        // 004C - 0050
            bool m_bInitialized;                         // 0050 - 0051
            char pad3[0x7];                              // 0051 - 0058
            C_GameTime *m_pGameTime;                     // 0058 - 0060
            ue::C_GameFSM *m_pGameFSM;                   // 0060 - 0068
            void *m_pGameContext;                        // 0068 - 0070
            ue::I_Engine *m_pEngine;                     // 0070 - 0078
            ue::sys::utils::C_PortCullis *m_pPortCullis; // 0078 - 0080
        };

        static_assert(sizeof(C_Application) == 0x80, "C_Application size is invalid");
    }
}
