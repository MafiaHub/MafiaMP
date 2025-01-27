#pragma once

namespace SDK {
    namespace ue::sys::core {
        class C_CameraComponent {
          public:
            char pad0[0x6C];                // 0000 - 006C
            float m_fAspectRatio;           // 006C - 0070
        };
    }
}
