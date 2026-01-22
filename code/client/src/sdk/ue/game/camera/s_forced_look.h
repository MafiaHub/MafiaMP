#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::game::camera {
        /**
         * S_ForcedLook - Forced look state structure
         * Size: 0x18 (24 bytes)
         */
        struct S_ForcedLook {
            char data[0x18];                                                // 0000 - 0018
        };

        static_assert(sizeof(S_ForcedLook) == 0x18, "S_ForcedLook size mismatch");
    } // namespace ue::game::camera
} // namespace SDK
