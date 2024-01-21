#include "c_streaming_module.h"

#include "../../patterns.h"

namespace SDK {
    namespace mafia::streaming {
        void C_StreamingModule::SetStreamingPosSource(E_StreamingPosSource source) {
            hook::this_call(gPatterns.C_StreamingModule__SetStreamingPosSource, this, source);
        }
    }; // namespace mafia::streaming
} // namespace SDK
