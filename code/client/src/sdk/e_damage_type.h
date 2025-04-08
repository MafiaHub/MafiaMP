#pragma once

namespace SDK {
    enum class E_DamageType {
        E_DT_UNDEFINED = -1,
        E_DT_SHOT      = 0,
        E_DT_EXPLOSION = 1,
        E_DT_CAR_EXPLOSION = 2,
        E_DT_MELEE         = 3,
        E_DT_TAKEDOWN_SPRINT = 4,
        E_DT_TAKEDOWN_STEALTH = 5,
        E_DT_TAKEDOWN_STUN    = 6,
        E_DT_TAKEDOWN_WEAPON  = 7,
        E_DT_FIRE             = 8,
        E_DT_SCRIPT           = 9,
        E_DT_IN_CAR           = 10,
        E_DT_COLLISION        = 11,
        E_DT_FALL             = 12,
        E_DT_CINEMATICDEADZONE = 13,
        E_DT_ANIMEVENT         = 14,
        E_DT_MAX
    };
}
