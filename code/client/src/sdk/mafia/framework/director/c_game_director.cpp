#include "c_game_director.h"

namespace SDK {
    namespace mafia::framework::director {
        C_GameDirector *C_GameDirector::GetInstance() {
            return hook::this_call<C_GameDirector *>(gPatterns.I_GameDirector__GetInstance);
        }

        int64_t *C_GameDirector::GetDistrict(ue::sys::math::C_Vector const &vec) {
            return hook::this_call<int64_t *>(gPatterns.C_GameDirector__GetDistrict, this, vec);
        }
    } // namespace mafia::framework::director
} // namespace SDK
