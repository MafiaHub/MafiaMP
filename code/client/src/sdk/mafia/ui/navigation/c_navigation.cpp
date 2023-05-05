#include "c_navigation.h"

#include <utils/hooking/hooking.h>

#include "../../../patterns.h"

namespace SDK {
    namespace mafia::ui::navigation {
        C_Navigation *mafia::ui::navigation::C_Navigation::GetInstance() {
            return hook::this_call<mafia::ui::navigation::C_Navigation *>(gPatterns.C_Navigation__GetInstance);
        }

        void C_Navigation::EnableGPSCustomPath(std::vector<ue::sys::math::C_Vector> const& vectors) {
            hook::this_call(gPatterns.C_Navigation__EnableGPSCustomPath, this, vectors);
        }

        void C_Navigation::SetUserMark(ue::sys::math::C_Vector2 const &worldPos, float &distance) {
            hook::this_call(gPatterns.C_Navigation__SetUserMark, this, worldPos, distance);
        }

        int64_t C_Navigation::RegisterHumanPlayer(I_Human2 *human) {
            return hook::this_call<int64_t>(gPatterns.C_Navigation__RegisterHumanPlayer, this, human);
        }

        int64_t C_Navigation::RegisterHumanPolice(I_Human2 *human) {
            return hook::this_call<int64_t>(gPatterns.C_Navigation__RegisterHumanPolice, this, human);
        }

        int64_t C_Navigation::RegisterVehicleCommon(C_Actor *ent) {
            return hook::this_call<int64_t>(gPatterns.C_Navigation__RegisterVehicleCommon, this, ent);
        }

        int64_t C_Navigation::RegisterVehicleEntity(void *ent, char const *name) {
            return hook::this_call<int64_t>(gPatterns.C_Navigation__RegisterVehicleEntity, this, ent, name);
        }

        int64_t C_Navigation::RegisterVehiclePolice(C_Actor *actor) {
            return hook::this_call<int64_t>(gPatterns.C_Navigation__RegisterVehiclePolice, this, actor);
        }

        int64_t C_Navigation::RegisterVehiclePoliceBoat(C_Actor *actor) {
            return hook::this_call<int64_t>(gPatterns.C_Navigation__RegisterVehiclePoliceBoat, this, actor);
        }

        int64_t C_Navigation::RegisterVehiclePoliceMoto(C_Actor *actor) {
            return hook::this_call<int64_t>(gPatterns.C_Navigation__RegisterVehiclePoliceMoto, this, actor);
        }

        int64_t C_Navigation::RegisterVehicleTaxi(C_Actor *actor) {
            return hook::this_call<int64_t>(gPatterns.C_Navigation__RegisterVehicleTaxi, this, actor);
        }

        bool C_Navigation::UnregisterId(unsigned int id) {
            return hook::this_call<bool>(gPatterns.C_Navigation__UnregisterId, this, id);
        }

        bool C_Navigation::UnregisterHuman(I_Human2* human, bool unk) {
            return hook::this_call<bool>(gPatterns.C_Navigation__UnregisterHuman, this, human, unk);
        }

        bool C_Navigation::UnregisterVehicle(C_Actor *ent) {
            return hook::this_call<bool>(gPatterns.C_Navigation__UnregisterVehicle, this, ent);
        }
    }
}
