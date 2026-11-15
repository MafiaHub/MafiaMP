#include "c_navigation.h"

#include "../../../patterns.h"

namespace SDK {
    namespace mafia::ui::navigation {
        C_Navigation *mafia::ui::navigation::C_Navigation::GetInstance() {
            return hook::this_call<mafia::ui::navigation::C_Navigation *>(gPatterns.C_Navigation__GetInstance);
        }

        void C_Navigation::EnableGPSCustomPath(std::vector<ue::sys::math::C_Vector> const &vectors) {
            hook::this_call(gPatterns.C_Navigation__EnableGPSCustomPath, this, vectors);
        }

        void C_Navigation::SetUserMark(ue::sys::math::C_Vector2 const &worldPos, float &distance) {
            hook::this_call(gPatterns.C_Navigation__SetUserMark, this, worldPos, distance);
        }

        S_Entity *C_Navigation::GetEntityById(uint32_t id) const {
            return hook::this_call<S_Entity *>(gPatterns.C_Navigation__GetEntityById, this, id);
        }

        S_Entity *C_Navigation::GetEntityByGameEntity(ue::sys::core::C_SceneObject const *sceneObject) const {
            return hook::this_call<S_Entity *>(gPatterns.C_Navigation__GetEntityByGameEntity, this, sceneObject);
        }

        uint32_t C_Navigation::RegisterIconPos(ue::sys::math::C_Vector const &position, char const *icon, uint64_t hash, bool flag, uint64_t hash2, ue::C_String const &string, uint16_t visibilityFlags, uint32_t color, char const *string2, char const *string3) {
            return hook::this_call<uint32_t>(gPatterns.C_Navigation__RegisterIconPos, this, &position, icon, hash, flag, hash2, &string, visibilityFlags, color, string2, string3);
        }

        bool C_Navigation::ChangeIconTargetToPos(uint32_t id, ue::sys::math::C_Vector const &position) {
            return hook::this_call<bool>(gPatterns.C_Navigation__ChangeIconTargetToPos, this, id, &position);
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

        uint32_t C_Navigation::RegisterVehicleEntity(ue::sys::core::C_SceneObject *sceneObject, char const *icon) {
            return hook::this_call<uint32_t>(gPatterns.C_Navigation__RegisterVehicleEntity, this, sceneObject, icon);
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

        bool C_Navigation::UnregisterHuman(I_Human2 *human, bool unk) {
            return hook::this_call<bool>(gPatterns.C_Navigation__UnregisterHuman, this, human, unk);
        }

        bool C_Navigation::UnregisterVehicle(C_Actor *ent) {
            return hook::this_call<bool>(gPatterns.C_Navigation__UnregisterVehicle, this, ent);
        }

        void C_Navigation::SelectObjective(uint32_t id, bool flag) {
            hook::this_call(gPatterns.C_Navigation__SelectObjective, this, id, flag);
        }

        void C_Navigation::ClearSelectedObjective(bool flag, bool flag2) {
            hook::this_call(gPatterns.C_Navigation__ClearSelectedObjective, this, flag, flag2);
        }
    } // namespace mafia::ui::navigation
} // namespace SDK
