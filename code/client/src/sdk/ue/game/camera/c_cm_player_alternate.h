#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::game::camera {
        class C_CM_PlayerAlternate {
          public:
            int64_t SendCommand(unsigned int, void *, void *);
        };
    }
}
