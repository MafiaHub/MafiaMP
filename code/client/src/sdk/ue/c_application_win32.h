#pragma once

#include <Windows.h>

#include "sdk/c_command_line.h"
#include "sdk/ue/c_application.h"
#include "sdk/ue/c_mutex.h"

namespace SDK {
    namespace ue {
        class C_Application_Win32: public C_Application {
          public:
            HINSTANCE m_hInstance;        // 0080 - 0088
            HINSTANCE m_hPrevInstance;    // 0088 - 0090
            LPSTR m_lpCmdLine;            // 0090 - 0098
            int32_t m_nCmdShow;           // 0098 - 009C
            int32_t m_nFPSMin;            // 009C - 00A0
            char pad0[0x4];               // 00A0 - 00A4
            int32_t m_nFPSMax;            // 00A4 - 00A8
            char pad1[0x28];              // 00A8 - 00D0
            HBITMAP m_hBitmap;            // 00D0 - 00D8
            HWND m_pWindow;               // 00D8 - 00E0
            char pad2[0x2];               // 00E0 - 00E2
            uint16_t m_nUnknown1;         // 00E2 - 00E4
            bool m_bUnknown1;             // 00E4 - 00E5
            bool m_bNeedsMouseClipUpdate; // 00E5 - 00E6
            bool m_bUnknown2;             // 00E6 - 00E7
            bool m_bUnknown3;             // 00E7 - 00E8
            FILTERKEYS m_filterKeys;      // 00E8 - 00F0
            TOGGLEKEYS m_toggleKeys;      // 00F0 - 00F8
            STICKYKEYS m_stickyKeys;      // 00F8 - 0110
            char m_sUniqueId[0x64];       // 0110 - 0174
            char pad3[0x4];               // 0174 - 0178
            void *m_pUnknown1;            // 0178 - 0180
            void *m_pUnknown2;            // 0180 - 0188
            bool m_bUnknown4;             // 0188 - 0189
            char pad4[0x7];               // 0189 - 0190
            void *m_pUnknown3;            // 0190 - 0198
            void *m_pUnknown4;            // 0198 - 01A0
            void *m_pUnknown5;            // 01A0 - 01A8
            ue::C_Mutex m_mutex;          // 01A8 - 01D0
            bool m_bUnknown5;             // 01D0 - 01D1
            char pad5[0x17];              // 01D1 - 01E8
        };

        static C_Application_Win32* GetApplicationWin32() {
            // TODO: move this to pattern
            return reinterpret_cast<C_Application_Win32 *>(0x0000001461F63E0);
        }

        static_assert(sizeof(C_Application_Win32) == 0x1E8, "C_Application_Win32 size is invalid");
    }
}
