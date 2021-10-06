#include "c_streaming_module.h"

#include <utils/hooking/hooking.h>

#include "../../patterns.h"

namespace SDK {
    namespace mafia::streaming {
        void C_StreamingModule::SetStreamingPosSource(E_StreamingPosSource source) {
            hook::this_call(gPatterns.C_StreamingModule__SetStreamingPosSourceAddr, this, source);
        }
    }; // namespace mafia::streaming
}
