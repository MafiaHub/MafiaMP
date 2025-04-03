#pragma once

#include <sol/sol.hpp>

#include "ui.h"

namespace MafiaMP::Scripting {
    class Builtins final {
      public:
        static void Register(sol::state* luaEngine) {
            if (!luaEngine) {
                return;
            }

            Scripting::UI::Register(luaEngine);
        }
    };
}
