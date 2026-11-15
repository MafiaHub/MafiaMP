#pragma once

#include "sdk/entities/c_actor.h"
#include "sdk/entities/c_human_2.h"
#include "sdk/mafia/ui/navigation/s_entity.h"
#include "sdk/ue/c_string.h"
#include "sdk/ue/sys/core/c_scene_object.h"
#include "sdk/ue/sys/math/c_vector.h"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace SDK {
    namespace mafia::ui::navigation {
        class C_Navigation {
          public:
            static C_Navigation *GetInstance();

            void EnableGPSCustomPath(std::vector<ue::sys::math::C_Vector> const &);

            void SetUserMark(ue::sys::math::C_Vector2 const &, float &);

            S_Entity *GetEntityById(uint32_t id) const;
            S_Entity *GetEntityByGameEntity(ue::sys::core::C_SceneObject const *sceneObject) const;

            // TODO: the two hashes, the flag and the three strings are not identified. Callers pass
            // what the game's five-argument script RegisterIconPos passes.
            uint32_t RegisterIconPos(ue::sys::math::C_Vector const &position, char const *icon, uint64_t, bool, uint64_t, ue::C_String const &, uint16_t visibilityFlags, uint32_t color, char const *, char const *);
            bool ChangeIconTargetToPos(uint32_t id, ue::sys::math::C_Vector const &position);

            int64_t RegisterHumanPlayer(I_Human2 *);
            int64_t RegisterHumanPolice(I_Human2 *);

            int64_t RegisterVehicleCommon(C_Actor *);
            // Reuses, and re-icons, the entity already registered for the scene object, if any.
            uint32_t RegisterVehicleEntity(ue::sys::core::C_SceneObject *sceneObject, char const *icon);
            int64_t RegisterVehiclePolice(C_Actor *);
            int64_t RegisterVehiclePoliceBoat(C_Actor *);
            int64_t RegisterVehiclePoliceMoto(C_Actor *);
            int64_t RegisterVehicleTaxi(C_Actor *);

            bool UnregisterId(unsigned int);
            bool UnregisterHuman(I_Human2 *, bool);
            bool UnregisterVehicle(C_Actor *);

            // The selected objective is the one the GPS routes to.
            void SelectObjective(uint32_t id, bool);
            void ClearSelectedObjective(bool, bool);
            uint32_t GetSelectedObjectiveId() const {
                return m_uSelectedObjectiveId;
            }

          private:
            char _pad0[0x1F4];               // 0x00 - 0x1F4
            uint32_t m_uSelectedObjectiveId; // 0x1F4 - 0x1F8 (SelectObjective writes it, ClearSelectedObjective reads it)

            static void AssertLayout() {
                static_assert(offsetof(C_Navigation, m_uSelectedObjectiveId) == 0x1F4, "C_Navigation::m_uSelectedObjectiveId offset mismatch");
            }
        };
    } // namespace mafia::ui::navigation
} // namespace SDK
