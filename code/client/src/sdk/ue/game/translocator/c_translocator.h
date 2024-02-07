#pragma once

#include "../../c_time_budget.h"
#include "../../sys/math/c_vector.h"

namespace SDK {
    namespace ue::game::translocator {
        class C_Translocator {
          public:
            struct S_ObjectToSpawn {
                char pad0[0x8];
                void *m_pUnk;                           // 0008 - 0010
                ue::sys::math::C_Vector m_vPosition;    // 0010 - 001C
                char pad1[0x10];                        // 001C - 002C
                int m_nObjectID;                        // 002C - 0030
            };
            void SpawnObject(S_ObjectToSpawn const& object);
            void SpawnObjects(ue::C_TimeBudget const& timeBudget);

            private:
                char pad0[0x48];            /// 0000 - 0048
        };
    }
}
