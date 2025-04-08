#include "sdk/patterns.h"

#include <utils/hooking/hook_function.h>

static InitFunction init([]() {
    /**
     * Disable traffic
     *
     * Traffic is automaticaly loaded by the game via C_StreamingTrafficModule::OpenSeason
     * after C_StreamMap::OpenPart("freeride") is called.
     */
    hook::return_function(SDK::gPatterns.C_StreamingTrafficModule__OpenSeason);
    },
    "Traffic");
