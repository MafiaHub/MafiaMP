#pragma once

#include "human.h"

namespace MafiaMP::Scripting {
    class Player final: public MafiaMP::Scripting::Human {
      public:
        Player(flecs::entity_t ent): Human(ent, true) {}

        std::string ToString() const override;

        static void EventPlayerConnected(flecs::entity e);

        static void EventPlayerDisconnected(flecs::entity e);

        static void EventPlayerDied(flecs::entity e);

        static void EventPlayerVehicleEnter(flecs::entity player, flecs::entity vehicle, int seatIndex);

        static void EventPlayerVehicleLeave(flecs::entity player, flecs::entity vehicle);

        void SendChat(std::string message);

        static void SendChatToAll(std::string message);

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule);

        static v8::Local<v8::Object> WrapPlayer(Framework::Scripting::Engines::Node::Engine *engine, flecs::entity e);
    };
} // namespace MafiaMP::Scripting
