#pragma once

#include <sol/sol.hpp>

#include "human.h"

namespace MafiaMP::Scripting {
    class Player final: public MafiaMP::Scripting::Human {
      public:
        Player(flecs::entity_t ent): Human(ent) {}

        Player(flecs::entity ent): Human(ent.id()) {}

        static void EventPlayerConnected(flecs::entity e);

        static void EventPlayerDisconnected(flecs::entity e);

        static void EventPlayerDied(flecs::entity e);

        std::string ToString() const override;

        void Destroy();

        void SendChat(std::string message);

        static void SendChatToAll(std::string message);

        static void Register(sol::state *luaEngine);
    };
} // namespace MafiaMP::Scripting
