#pragma once

#include <cstdint>

#include "sdk/ue/c_time_budget.h"
#include "sdk/ue/sys/math/c_vector.h"

namespace SDK {
    namespace ue::game::translocator {
        class C_Translocator {
          public:
            struct S_ObjectToSpawn {
                void *pObject;                    // 0000 - 0008
                uint32_t index;                   // 0x08 - 000C
                uint32_t unk1;                    // 0x0C - 0010
                SDK::ue::sys::math::C_Vector pos; // 0x10 - 001C
            };
            void SpawnObject(S_ObjectToSpawn const& object);
            void SpawnObjects(ue::C_TimeBudget const& timeBudget);

            private:
                char pad0[0x48];            /// 0000 - 0048
        };

        static_assert(sizeof(C_Translocator::S_ObjectToSpawn) == 0x20, "C_Translocator::S_ObjectToSpawn is not the correct size");
    }
}
