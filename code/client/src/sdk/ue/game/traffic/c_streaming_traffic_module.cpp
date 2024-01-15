#include "c_streaming_traffic_module.h"

#include "../../../patterns.h"

#include <utils/hooking/hooking.h>

namespace ue::game::traffic {
    void C_StreamingTrafficModule::SetMaxHumanElements(int64_t maxHumanElements) {
        hook::this_call<void>(gPatterns.C_StreamingTrafficModule__SetMaxHumanElements, this, maxHumanElements);
    }

    C_StreamingTrafficModule *C_StreamingTrafficModule::GetInstance() {
        return hook::this_call<C_StreamingTrafficModule *>(gPatterns.C_StreamingTrafficModule__GetInstance);
    }
}