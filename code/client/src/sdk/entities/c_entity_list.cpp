#include "c_entity_list.h"

#include "../patterns.h"

namespace SDK {
    C_EntityList *GetEntityList() {
        return hook::this_call<C_EntityList *>(gPatterns.C_EntityList__GetEntityList);
    }
} // namespace SDK
