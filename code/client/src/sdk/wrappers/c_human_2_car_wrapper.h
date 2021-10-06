#pragma once

#include "../entities/c_actor.h"

namespace SDK {
    class C_Human2CarWrapper {
      public:
        bool IsDriver(C_Actor *);
        unsigned int GetSeatID(C_Actor *);
    };
}
