#pragma once

#include <sol/sol.hpp>

#include "integrations/server/scripting/builtins/entity.h"
#include "shared/modules/human_sync.hpp"
#include "scripting/server_engine.h"

namespace MafiaMP::Scripting {
    class Vehicle;
    class Human final: public Framework::Integrations::Scripting::Entity {
      public:
        Human(flecs::entity_t ent): Entity(ent) {
            const auto humanData = _ent.get<Shared::Modules::HumanSync::UpdateData>();

            if (!humanData) {
                throw std::runtime_error(fmt::format("Entity handle '{}' is not a Human!", ent));
            }
        }

        Human(flecs::entity ent): Human(ent.id()) {}

        static void EventPlayerDied(flecs::entity e);
        static void EventPlayerConnected(flecs::entity e);
        static void EventPlayerDisconnected(flecs::entity e);

        static void Register(sol::state &luaEngine);

        std::string ToString() const override;

        void Destroy();

        void AddWeapon(int weaponId, int ammo);

        void SendChat(std::string message);

        Vehicle GetVehicle() const;
        int GetVehicleSeat() const;

        static void SendChatToAll(std::string message);

        void SetHealth(float health);
        float GetHealth() const;
    };
} // namespace MafiaMP::Scripting
