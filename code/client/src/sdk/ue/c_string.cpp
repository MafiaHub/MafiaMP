#include "c_string.h"

#include "../patterns.h"

namespace SDK {
    namespace ue {
        void C_String::SetString(const char *str) {
            hook::this_call(gPatterns.C_String__SetString, this, str);
        }
    } // namespace ue
} // namespace SDK
