#pragma once

#include "c_entity.h"

namespace SDK {
    // Size: 0xE0 (224 bytes) - same as C_Entity, only virtual method overrides
    class C_EntityPos : public C_Entity {};

    static_assert(sizeof(C_EntityPos) == 0xE0, "C_EntityPos size mismatch");
} // namespace SDK
