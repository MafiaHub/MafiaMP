#pragma once

namespace SDK {
    namespace ue {
        class C_Mutex {
          private:
            char pad[0x28]; // 0x28 bytes (40 bytes) to match actual size 0x1A8 - 0x1D0
        };
    }
}
