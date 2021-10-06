#include "i_core.h"

#include "../../../patterns.h"

#include <utils/hooking/hooking.h>

namespace SDK {
    namespace ue::sys::core {
        C_Core *I_Core::GetInstance() {
            return hook::this_call<C_Core *>(gPatterns.I_Core__GetInstance);
        }
    } // namespace ue::sys::core
} // namespace SDK
