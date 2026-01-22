#pragma once

#include <Windows.h>

#include "sdk/c_command_line.h"
#include "sdk/ue/c_application.h"
#include "sdk/ue/c_mutex.h"

namespace SDK {
    namespace ue {
        struct S_WindowsStartSetup {
            int32_t m_nWindowX;          // 0000 - 0004 (saved window position X)
            int32_t m_nWindowY;          // 0004 - 0008 (saved window position Y)
            int32_t m_nWidth;            // 0008 - 000C (default 800)
            int32_t m_nHeight;           // 000C - 0010 (default 600)
            int32_t m_nDisplayMode;      // 0010 - 0014 (fullscreen flag, default 1)
            int32_t m_nVSync;            // 0014 - 0018 (default 1)
            int32_t m_nRefreshRateLo;    // 0018 - 001C (default 1)
            int32_t m_nRefreshRateHi;    // 001C - 0020 (default 0)
            int32_t m_nMonitorIndexLo;   // 0020 - 0024 (default 0)
            int32_t m_nMonitorIndexHi;   // 0024 - 0028 (default 0)
            char pad0[0x4];              // 0028 - 002C
            int32_t m_nAntiAliasing;     // 002C - 0030 (default 1)
            bool m_bInitialized;         // 0030 - 0031 (default false)
            char pad1[0x3];              // 0031 - 0034
        };
        static_assert(sizeof(S_WindowsStartSetup) == 0x34, "S_WindowsStartSetup size is invalid");

        struct S_WindowMessage {
            int32_t m_nMessage;          // 0000 - 0004 (WM_* message type)
            char pad0[0x4];              // 0004 - 0008
            int64_t m_nParam;            // 0008 - 0010 (WPARAM/LPARAM combined)
            int64_t m_nExtra;            // 0010 - 0018
        };
        static_assert(sizeof(S_WindowMessage) == 0x18, "S_WindowMessage size is invalid");

        struct S_FilterKeys {
            uint32_t cbSize;             // 0000 - 0004
            uint32_t dwFlags;            // 0004 - 0008
        };
        static_assert(sizeof(S_FilterKeys) == 0x8, "S_FilterKeys size is invalid");

        struct S_ToggleKeys {
            uint32_t cbSize;             // 0000 - 0004
            uint32_t dwFlags;            // 0004 - 0008
        };
        static_assert(sizeof(S_ToggleKeys) == 0x8, "S_ToggleKeys size is invalid");

        struct S_StickyKeys {
            uint32_t cbSize;             // 0000 - 0004
            uint32_t dwFlags;            // 0004 - 0008
            char pad[0x10];              // 0008 - 0018
        };
        static_assert(sizeof(S_StickyKeys) == 0x18, "S_StickyKeys size is invalid");

        class C_Application_Win32: public C_Application {
          public:
            HINSTANCE m_hInstance;               // 0080 - 0088
            HINSTANCE m_hPrevInstance;           // 0088 - 0090
            LPSTR m_lpCmdLine;                   // 0090 - 0098
            int32_t m_nCmdShow;                  // 0098 - 009C
            S_WindowsStartSetup m_windowSetup;   // 009C - 00D0 (window startup configuration)
            HBITMAP m_hSplashBitmap;             // 00D0 - 00D8 (splash screen bitmap)
            HWND m_hWindow;                      // 00D8 - 00E0
            bool m_bHasFocus;                    // 00E0 - 00E1 (window has focus)
            bool m_bSuspended;                   // 00E1 - 00E2 (application suspended)
            bool m_bSavedFullscreenState;        // 00E2 - 00E3 (saved fullscreen state for restore)
            bool m_bFullscreenStateSaved;        // 00E3 - 00E4 (flag: fullscreen state has been saved)
            bool m_bMinimized;                   // 00E4 - 00E5 (window is minimized)
            bool m_bNeedsMouseClipUpdate;        // 00E5 - 00E6 (mouse clip rect needs update)
            bool m_bUnknown1;                    // 00E6 - 00E7
            bool m_bWasFocusLost;                // 00E7 - 00E8 (focus was lost, used for state tracking)
            S_FilterKeys m_filterKeys;           // 00E8 - 00F0 (saved accessibility settings)
            S_ToggleKeys m_toggleKeys;           // 00F0 - 00F8
            S_StickyKeys m_stickyKeys;           // 00F8 - 0110
            char m_sUniqueId[0x64];              // 0110 - 0174 (unique application ID from crash handler)
            char pad1[0x4];                      // 0174 - 0178
            void *m_pInputLayerContext;          // 0178 - 0180 (context for input layer listener)
            void *m_pInputLayerListenerVtable;   // 0180 - 0188 (C_InputLayerListener vtable)
            bool m_bInputLayerRegistered;        // 0188 - 0189 (input layer listener registered)
            char pad2[0x7];                      // 0189 - 0190
            S_WindowMessage *m_pMsgQueueBegin;   // 0190 - 0198 (window message queue start)
            S_WindowMessage *m_pMsgQueueEnd;     // 0198 - 01A0 (window message queue current end)
            S_WindowMessage *m_pMsgQueueCapacity;// 01A0 - 01A8 (window message queue capacity)
            ue::C_Mutex m_mutex;                 // 01A8 - 01D0 (mutex for message queue access)
            bool m_bNeedsInputRefresh;           // 01D0 - 01D1 (set on WM_DEVICECHANGE, triggers input device refresh)
            char pad3[0x17];                     // 01D1 - 01E8
        };

        static C_Application_Win32* GetApplicationWin32() {
            // TODO: move this to pattern
            return reinterpret_cast<C_Application_Win32 *>(0x0000001461F63E0);
        }

        static_assert(sizeof(C_Application_Win32) == 0x1E8, "C_Application_Win32 size is invalid");
    }
}
