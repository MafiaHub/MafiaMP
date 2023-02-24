#pragma once

#include "c_fire.h"
#include "c_explosion.h"

namespace SDK {
    namespace ue::game::shotmanager {
        struct S_FireInit {

        };

        struct S_ExplosionInit {

        };
        class C_ShotManager {
          public:
            static C_ShotManager *GetInstance();

            C_Fire *CreateFire(S_FireInit const *);
            C_Explosion *CreateExplosion(S_ExplosionInit const *);
        };
    }
}
