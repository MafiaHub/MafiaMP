#pragma once

namespace SDK {
    namespace ue::sys::utils {
        class C_SimpleTimer {
          public:
            virtual ~C_SimpleTimer() = default;

            // vtable at 0x00
            void *vtable;        // 0x00
            uint8_t padding[64]; // 0x08 to 0x48
            uint64_t timerField; // 0x48 (0xB0 - 0x70 = 0x40)

            C_SimpleTimer() {
                vtable     = (void *)0x144B68D60; // From assembly
                timerField = 0;                   // Explicitly cleared in assembly
                memset(padding, 0, sizeof(padding));
            }
        };
    }
}
