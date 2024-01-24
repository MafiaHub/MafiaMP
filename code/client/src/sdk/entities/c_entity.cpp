#include "c_entity.h"

#include "../patterns.h"

namespace SDK {
    void C_Entity::GameInit() {
        hook::this_call<void>(gPatterns.C_Entity__GameInit, this);
    }

    void C_Entity::GameDone() {
        hook::this_call<void>(gPatterns.C_Entity__GameDone, this);
    }

    void C_Entity::Activate() {
        hook::this_call<void>(gPatterns.C_Entity__Activate, this);
    }

    void C_Entity::Deactivate() {
        hook::this_call<void>(gPatterns.C_Entity__Deactivate, this);
    }

    void C_Entity::Release() {
        hook::this_call<void>(gPatterns.C_Entity__Release, this);
    }
} // namespace SDK
