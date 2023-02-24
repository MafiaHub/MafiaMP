#include "c_explosion.h"

#include <utils/hooking/hooking.h>
#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::shotmanager {
        void C_Explosion::Clear() {
            hook::this_call(gPatterns.C_Explosion__Clear, this);
        }
    }
}
