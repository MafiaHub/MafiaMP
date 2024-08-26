#pragma once

#include <sol/sol.hpp>

#include "scripting/server_engine.h"

#include "../modules/environment.h"
#include "../modules/vehicle.h"

#include "../../shared/rpc/environment.h"

#include "vehicle.h"

#include "core_modules.h"

namespace MafiaMP::Scripting {
    class World final {
      public:
        static void SetWeather(std::string weatherSetName) {
            auto world = Framework::CoreModules::GetWorldEngine()->GetWorld();

            auto weather             = world->get_mut<Core::Modules::Environment::Weather>();
            weather->_weatherSetName = weatherSetName;
            FW_SEND_COMPONENT_RPC(MafiaMP::Shared::RPC::SetEnvironment, SLNet::RakString(weather->_weatherSetName.c_str()), {});
        }

        static void SetDayTimeHours(float dayTimeHours) {
            auto world = Framework::CoreModules::GetWorldEngine()->GetWorld();

            auto weather           = world->get_mut<Core::Modules::Environment::Weather>();
            weather->_dayTimeHours = dayTimeHours;
            FW_SEND_COMPONENT_RPC(MafiaMP::Shared::RPC::SetEnvironment, {}, weather->_dayTimeHours);
        }

        static void Register(sol::state &luaEngine) {
            sol::usertype<World> cls = luaEngine.new_usertype<World>("World");
            cls["setWeather"]        = &World::SetWeather;
            cls["setDayTimeHours"]   = &World::SetDayTimeHours;
        }
    };
} // namespace MafiaMP::Scripting
