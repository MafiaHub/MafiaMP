#pragma once

#include <dxgi.h>

namespace SDK {
    namespace ue::sys::render::device {
        class C_D3D11WindowContextCache {
          public:
              struct S_WndContextDesc {
                char pad0[0x20];                    // 0000 - 0020
                  IDXGISwapChain *m_pSwapChain;     // 0020 - 0024
            };
        };
    }
}
