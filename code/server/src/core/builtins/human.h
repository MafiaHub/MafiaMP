#pragma once

#include <sol/sol.hpp>

#include "integrations/server/scripting/builtins/entity.h"

#include "shared/modules/human_sync.hpp"

namespace MafiaMP::Scripting {
    class Vehicle;
    class Human: public Framework::Integrations::Scripting::Entity {
      public:
        Human(flecs::entity_t ent): Entity(ent) {
            const auto humanData = _ent.get<Shared::Modules::HumanSync::UpdateData>();

            if (!humanData) {
                throw std::runtime_error(fmt::format("Entity handle '{}' is not a Human!", ent));
            }
        }

        Human(flecs::entity ent): Human(ent.id()) {}

        std::string ToString() const override;

        void AddWeapon(int weaponId, int ammo);

        float GetHealth() const;

        void SetHealth(float health);

        sol::optional<Vehicle> GetVehicle() const;

        int GetVehicleSeatIndex() const;

        static void Register(sol::state &luaEngine);
    };
} // namespace MafiaMP::Scripting
