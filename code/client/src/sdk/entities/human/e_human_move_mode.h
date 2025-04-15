#pragma once

namespace SDK {
    enum class E_HumanMoveMode {
        E_HMM_NONE   = -1,
        E_HMM_BREATH = 0,
        E_HMM_STEP,
        E_HMM_WALK_SLOW,
        E_HMM_WALK,
        E_HMM_WALK_FAST,
        E_HMM_RUN,
        E_HMM_SPRINT,
        E_HMM_AUTO_SLOW,
        E_HMM_AUTO_NORMAL,
        E_HMM_AUTO_FAST,
        E_HMM_END
    };
}
