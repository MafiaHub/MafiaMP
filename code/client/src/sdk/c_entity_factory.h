#pragma once

#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include "entities/c_entity.h"

#include "patterns.h"

namespace SDK {
    class C_EntityFactoryBaseRecord {};

    class C_EntityFactory {
      public:
        static bool RegisterEntity(C_EntityFactoryBaseRecord *rec) {
            return hook::call<bool, C_EntityFactoryBaseRecord *>(gPatterns.C_EntityFactory__RegisterEntity, rec);
        }

        template <typename entityClass>
        static entityClass CreateEntity(E_EntityType type) {
            return hook::call<entityClass, E_EntityTypes>(gPatterns.C_EntityFactory__CreateEntity, type);
        };
    };
} // namespace SDK
