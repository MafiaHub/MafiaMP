#include "c_translocator_module.h"

namespace SDK {
    namespace ue::game::translocator {
        C_TranslocatorModule *GetTranslocatorModule() {
            // Directly cast the address of the module to the correct type
            return reinterpret_cast<C_TranslocatorModule *>(0x00000014636C1D0);
        }
    }
}
