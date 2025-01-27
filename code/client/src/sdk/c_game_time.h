#pragma once

#include <cstdint>

namespace SDK {
    class C_GameTime {
      public:
        uint64_t m_iTimerValue;   // 0000 - 0008
        int32_t m_iFrameTime;     // 0008 - 000C
        int32_t m_iProcessTimeMs; // 000C - 0010
        bool m_bSyncEnabled;      // 0010 - 0011
        char pad0[0x7];           // 0011 - 0018
    };

    static_assert(sizeof(C_GameTime) == 0x18, "C_GameTime size is invalid");
}
