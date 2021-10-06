#include "c_ticked_module_manager.h"

#include "patterns.h"

namespace SDK {
    C_TickedModuleManager *GetTickedModuleManager() {
        return hook::call<C_TickedModuleManager *>(gPatterns.C_TickedModuleManager__GetTickedModuleManager);
    }
} // namespace SDK