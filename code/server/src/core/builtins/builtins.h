#pragma once

#include <sol/sol.hpp>

#include "scripting/server_engine.h"

#include "chat.h"
#include "human.h"
#include "player.h"
#include "vehicle.h"
#include "world.h"

namespace MafiaMP::Scripting {
    class Builtins final {
      public:
        static void Register(sol::state *luaEngine) {
            Scripting::Chat::Register(luaEngine);
            Scripting::Human::Register(luaEngine);
            Scripting::Player::Register(luaEngine);
            Scripting::Vehicle::Register(luaEngine);
            Scripting::World::Register(luaEngine);
        }
    };
} // namespace MafiaMP::Scripting
