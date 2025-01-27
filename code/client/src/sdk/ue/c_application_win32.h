#pragma once

#include <Windows.h>

#include "sdk/c_command_line.h"
#include "sdk/ue/c_application.h"
#include "sdk/ue/c_mutex.h"

namespace SDK {
    namespace ue {
        class C_Application_Win32 : public C_Application {
          public:
            char pad0[0x08];                    // 0008 - 0010
            C_CommandLine *m_pCommandLine;      // 0010 - 0018
            int32_t m_nFPSMin;                  // 0018 - 001C
            char pad1[0x08];                    // 001C - 0024
            int32_t m_nFPSMax;                  // 0024 - 0028
            char pad2[0xA8];                    // 0028 - 00D0
            HBITMAP m_hBitmap;                  // 00D0 - 00D8
            HWND m_pWindow;                     // 00D8 - 00E0
            char pad3[0x05];                    // 00E0 - 00E5
            bool m_bNeedsMouseClipUpdate;       // 00E5 - 00E6
            char pad4[0x02];                    // 00E6 - 00E8
            FILTERKEYS m_filterKeys;            // 00E8 - 00F0
            TOGGLEKEYS m_toggleKeys;            // 00F0 - 00F8
            STICKYKEYS m_stickyKeys;            // 00F8 - 0100
            char pad5[0x08];                    // 0100 - 0108
            double m_dAccessibilityValue;       // 0108 - 0110
            char m_sUniqueId[0x64];             // 0110 - 0174 - Not yet confirmed size
            char pad6[0x34];                    // 0174 - 01A8
            ue::C_Mutex m_mutex;                // 01A8 - 01D0
        };
    }
}
