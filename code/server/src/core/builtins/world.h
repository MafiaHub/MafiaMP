#pragma once

#include <sol/sol.hpp>

#include "core/server.h"
#include "core_modules.h"

#include "core/modules/environment.h"
#include "core/modules/vehicle.h"

#include "shared/rpc/environment.h"

#include "vehicle.h"

namespace MafiaMP::Scripting {
    class World final {
      public:
        static Vehicle CreateVehicle(std::string modelName) {
            auto e = MafiaMP::Core::Modules::Vehicle::Create(Server::_serverRef);

            auto &frame     = e.ensure<Framework::World::Modules::Base::Frame>();
            frame.modelName = modelName;

            return Vehicle(e);
        }

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
            cls["createVehicle"]     = &World::CreateVehicle;
            cls["setWeather"]        = &World::SetWeather;
            cls["setDayTimeHours"]   = &World::SetDayTimeHours;
        }
    };
} // namespace MafiaMP::Scripting
