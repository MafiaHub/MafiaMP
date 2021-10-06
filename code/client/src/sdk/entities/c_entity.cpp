#include "c_entity.h"

#include <utils/hooking/hooking.h>
#include "../patterns.h"

namespace SDK {
    void C_Entity::GameInit() {
        hook::this_call<void>(gPatterns.C_Entity__GameInitAddr, this);
    }

    void C_Entity::GameDone() {
        hook::this_call<void>(gPatterns.C_Entity__GameDoneAddr, this);
    }

    void C_Entity::Activate() {
        hook::this_call<void>(gPatterns.C_Entity__ActivateAddr, this);
    }

    void C_Entity::Deactivate() {
        hook::this_call<void>(gPatterns.C_Entity__DeactivateAddr, this);
    }

    void C_Entity::Release() {
        hook::this_call<void>(gPatterns.C_Entity__ReleaseAddr, this);
    }
}
