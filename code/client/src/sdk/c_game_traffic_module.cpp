#include "c_game_traffic_module.h"

#include "patterns.h"

namespace SDK {
    C_GameTrafficModule *C_GameTrafficModule::GetInstance() {
        return hook::this_call<C_GameTrafficModule *>(gPatterns.C_GameTrafficModule__GetInstance);
    }
} // namespace SDK
