#pragma once

#include "../../../ue/sys/math/c_vector.h"

namespace SDK {
    class C_Actor;
    namespace mafia::ui::navigation {
        class C_Navigation {
          public:
            static C_Navigation *GetInstance();
            void SetUserMark(ue::sys::math::C_Vector2 const &, float &);
            void RegisterVehicleCommon(C_Actor *);
            void RegisterVehicleEntity(void *, char const *);
            void UnregisterVehicle(C_Actor *);
        };
    } // namespace mafia::ui::navigation
}
