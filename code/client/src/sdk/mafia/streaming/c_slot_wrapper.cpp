#include "c_slot_wrapper.h"

#include "../../patterns.h"

namespace SDK {
    namespace mafia::streaming {
        bool C_SlotWrapper::LoadData(char const *a2, ue::sys::core::C_Scene *a3, unsigned int a4, char const *a5, bool *a6, bool a7) {
            return hook::this_call<bool>(gPatterns.C_SlotWrapper__LoadData, this, a2, a3, a4, a5, a6, a7);
        }
    } // namespace mafia::streaming
} // namespace SDK
