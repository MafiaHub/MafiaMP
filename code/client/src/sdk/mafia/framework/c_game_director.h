#pragma once

#include <cstdint>

#include "../../entities/c_entity.h"
#include "../../ue/sys/math/c_vector.h"

namespace SDK {
    namespace mafia::framework::director {
        class I_GameDirector {
            class C_DistrictDefinition {

            };

          public:
        };

        class C_GameDirector {
          public:
            static C_GameDirector *GetInstance();
            int64_t *GetDistrict(ue::sys::math::C_Vector const &);
        };
    }; // namespace mafia::framework::director
};    // namespace SDK
