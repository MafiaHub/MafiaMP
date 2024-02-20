#pragma once

#include "integrations/server/scripting/builtins/node/entity.h"
#include "scripting/engines/node/engine.h"

namespace MafiaMP::Scripting {
    class Human final: public Framework::Integrations::Scripting::Entity {
      public:
        Human(flecs::entity_t ent): Entity(ent) {}

        static void EventPlayerDied(flecs::entity e);
        static void EventPlayerConnected(flecs::entity e);
        static void EventPlayerDisconnected(flecs::entity e);
        static void EventPlayerVehicleEnter(flecs::entity player, flecs::entity vehicle, int seatIndex);
        static void EventPlayerVehicleLeave(flecs::entity player, flecs::entity vehicle);

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule);

        static v8::Local<v8::Object> WrapHuman(Framework::Scripting::Engines::Node::Engine *engine, flecs::entity e);

        std::string ToString() const override;

        void Destroy(v8::Isolate *isolate);

        void AddWeapon(int weaponId, int ammo);

        void SendChat(std::string message);

        v8::Local<v8::Value> GetVehicle() const;
        int GetVehicleSeat() const;

        static void SendChatToAll(std::string message);

        void SetHealth(float health);
        float GetHealth() const;
    };
} // namespace MafiaMP::Scripting
