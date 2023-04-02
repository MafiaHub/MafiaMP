#pragma once

#include <cstdint>

#include "../../../entities/c_entity.h"
#include "../../../ue/sys/math/c_vector.h"

namespace SDK {
    namespace mafia::framework::director {
        class I_GameDirector {
          public:
            class C_DistrictDefinition {
              public:
                char pad0[0x10];     // 0000 - 0010
                uint64_t districtID; // 0010 - 0014
            };
        };

        class C_GameDirector {
          public:
            static C_GameDirector *GetInstance();
            int64_t *GetDistrict(ue::sys::math::C_Vector const &);
        };
    }; // namespace mafia::framework::director
};    // namespace SDK
