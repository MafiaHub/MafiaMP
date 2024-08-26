#pragma once

#include <sol/sol.hpp>

#include "integrations/server/scripting/builtins/node/entity.h"
#include "scripting/server_engine.h"

namespace MafiaMP::Scripting {
    class Human final: public Framework::Integrations::Scripting::Entity {
      public:
        Human(flecs::entity_t ent): Entity(ent) {}

        static void EventPlayerDied(flecs::entity e);
        static void EventPlayerConnected(flecs::entity e);
        static void EventPlayerDisconnected(flecs::entity e);

        static void Register(sol::state &luaEngine);

        //static v8::Local<v8::Object> WrapHuman(Framework::Scripting::Engine *engine, flecs::entity e);

        std::string ToString() const override;

        void Destroy();

        void AddWeapon(int weaponId, int ammo);

        void SendChat(std::string message);

        // v8::Local<v8::Value> GetVehicle() const;
        int GetVehicleSeat() const;

        static void SendChatToAll(std::string message);

        void SetHealth(float health);
        float GetHealth() const;
    };
} // namespace MafiaMP::Scripting
