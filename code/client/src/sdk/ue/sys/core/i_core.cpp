#include "i_core.h"

#include <utils/hooking/hooking.h>
#include "../../patterns.h"

namespace SDK {
    namespace ue::sys::core {
    C_Core* I_Core::GetInstance() {
        return hook::this_call<C_Core*>(gPatterns.I_Core__GetInstance);
    }
}
}