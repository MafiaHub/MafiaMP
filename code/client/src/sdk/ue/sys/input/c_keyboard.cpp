#include "c_keyboard.h"

#include "sdk/patterns.h"

namespace SDK {
    namespace ue::sys::input {
        uint32_t C_Keyboard::GetActiveModifiers() const {
            return hook::this_call<uint32_t>(gPatterns.C_Keyboard__GetActiveModifiers, this);
        }

        int64_t C_Keyboard::ToString(char *a1) {
            return hook::this_call<int64_t>(gPatterns.C_Keyboard__ToString, this, a1);
        }
    }
}
