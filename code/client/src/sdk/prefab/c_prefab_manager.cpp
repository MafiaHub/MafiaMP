#include "c_prefab_manager.h"

#include "../patterns.h"

namespace SDK {
    namespace prefab {
        C_PrefabManager *C_PrefabManager::GetInstance() {
            return hook::call<C_PrefabManager *>(gPatterns.C_PrefabManager__GetInstance);
        }
    } // namespace prefab
} // namespace SDK
