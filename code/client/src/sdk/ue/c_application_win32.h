#pragma once

#include <Windows.h>

namespace SDK {
    namespace ue {
        class C_Application_Win32 {
          public:
            char pad0[0x0D8];       // 0000 - 00D8
            HWND m_pWindow;
        };
    }
}
