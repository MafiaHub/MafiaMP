#include "c_navigation.h"

#include <utils/hooking/hooking.h>

#include "../../../patterns.h"

namespace SDK {
    namespace mafia::ui::navigation {
        C_Navigation *mafia::ui::navigation::C_Navigation::GetInstance() {
            return hook::this_call<mafia::ui::navigation::C_Navigation *>(gPatterns.C_Navigation__GetInstance);
        }

        void C_Navigation::SetUserMark(ue::sys::math::C_Vector2 const &worldPos, float &distance) {
            hook::this_call(gPatterns.C_Navigation__SetUserMark, this, worldPos, distance);
        }

        void C_Navigation::RegisterVehicleCommon(C_Actor *ent) {
            hook::this_call(gPatterns.C_Navigation__RegisterVehicleCommon, this, ent);
        }

        void C_Navigation::RegisterVehicleEntity(void *ent, char const *name) {
            hook::this_call(gPatterns.C_Navigation__RegisterVehicleEntity, this, ent, name);
        }

        void C_Navigation::UnregisterVehicle(C_Actor *ent) {
            hook::this_call(gPatterns.C_Navigation__UnregisterVehicle, this, ent);
        }
    }
}
