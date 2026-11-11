#pragma once

#include "../../../ue/sys/math/c_vector.h"

#include <vector>

namespace SDK {
    class C_Actor;
    class C_Human2;
    class I_Human2;
    namespace ue::sys::core {
        class C_SceneObject;
    }
    namespace mafia::ui::navigation {
        class C_Navigation {
          public:
            static C_Navigation *GetInstance();

            void EnableGPSCustomPath(std::vector<ue::sys::math::C_Vector> const &);

            void SetUserMark(ue::sys::math::C_Vector2 const &, float &);

            uint32_t RegisterHumanEntity(C_Human2 *, int, int, bool);
            int64_t RegisterHumanPlayer(I_Human2 *);
            int64_t RegisterHumanPolice(I_Human2 *);

            int64_t RegisterVehicleCommon(C_Actor *);
            uint32_t RegisterVehicleEntity(ue::sys::core::C_SceneObject *, char const *);
            int64_t RegisterVehiclePolice(C_Actor *);
            int64_t RegisterVehiclePoliceBoat(C_Actor *);
            int64_t RegisterVehiclePoliceMoto(C_Actor *);
            int64_t RegisterVehicleTaxi(C_Actor *);

            bool UnregisterId(unsigned int);
            bool UnregisterHuman(I_Human2 *, bool);
            bool UnregisterVehicle(C_Actor *);
        };
    } // namespace mafia::ui::navigation
}
