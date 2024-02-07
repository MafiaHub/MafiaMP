#pragma once

#include "../entities/c_car.h"
#include "../entities/c_actor.h"

namespace SDK {
    class C_Human2CarWrapper {
      public:
        struct S_SeatInfo;

      public:
        char pad0[0x18];                    // 0000 - 0018
        C_Car *m_pUsedCar;                  // 0018 - 0020
      public:
        bool IsDriver(C_Actor *);
        bool IsEngineOn();
        unsigned int GetSeatID(C_Actor *);
    };
}
