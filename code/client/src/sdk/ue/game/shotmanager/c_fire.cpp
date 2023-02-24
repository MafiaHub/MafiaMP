#include "c_fire.h"

#include <utils/hooking/hooking.h>
#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::shotmanager {
        void C_Fire::Clear() {
            hook::this_call<void>(gPatterns.C_Fire__Clear, this);
        }
    }
}
