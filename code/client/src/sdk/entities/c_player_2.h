#pragma once

#include "c_human_2.h"

namespace SDK {
    class C_Player2: public C_Human2 {
      public:
        virtual bool UsingMountedWeapon() = 0;
    };
}
