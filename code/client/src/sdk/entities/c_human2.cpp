#include "../patterns.h"

#include "c_human_2.h"

namespace SDK {
    void C_Human2::EnableShadows(bool bEnable) {
        hook::this_call<void>(gPatterns.C_Human2__EnableShadows, this, bEnable);
    }

    void C_Human2::EnableHumanClothes() {
        hook::this_call<void>(gPatterns.C_Human2__EnableHumanClothes, this);
    }
} // namespace SDK
