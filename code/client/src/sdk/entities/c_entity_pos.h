#pragma once

#include "c_entity.h"

namespace SDK {
    /**
     * C_EntityPos - Entity with position/direction support
     *
     * Inherits from C_Entity, adds no new members.
     * Only overrides virtual methods for position/direction handling.
     *
     * Size: 0xE0 (224 bytes) - same as C_Entity
     */
    class C_EntityPos : public C_Entity {
        // No additional members - only virtual method overrides
    };

    static_assert(sizeof(C_EntityPos) == 0xE0, "C_EntityPos size mismatch");
} // namespace SDK
