#include "c_streaming_traffic_module.h"

#include <utils/hooking/hooking.h>

namespace ue::game::traffic {
    void C_StreamingTrafficModule::SetMaxHumanElements(int64_t maxHumanElements) {
        // TODO: move to pattern
        hook::this_call<void>(0x000000142DA1C50, this, maxHumanElements);
    }

    C_StreamingTrafficModule *C_StreamingTrafficModule::GetInstance() {
        // TODO: move to pattern
        return hook::this_call<C_StreamingTrafficModule *>(0x000000142D954A0);
    }
}