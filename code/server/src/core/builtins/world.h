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

        static float GetDayTimeHours() {
            auto world = Framework::CoreModules::GetWorldEngine()->GetWorld();

            auto weather = world->get_mut<Core::Modules::Environment::Weather>();
            return weather->_dayTimeHours;
        }

        static void SetDayTimeHours(float dayTimeHours) {
            auto world = Framework::CoreModules::GetWorldEngine()->GetWorld();

            auto weather           = world->get_mut<Core::Modules::Environment::Weather>();
            weather->_dayTimeHours = dayTimeHours;
            FW_SEND_COMPONENT_RPC(MafiaMP::Shared::RPC::SetEnvironment, {}, weather->_dayTimeHours);
        }

        static std::string GetWeatherSet() {
            auto world = Framework::CoreModules::GetWorldEngine()->GetWorld();

            auto weather = world->get_mut<Core::Modules::Environment::Weather>();
            return weather->_weatherSetName;
        }

        static void SetWeatherSet(std::string weatherSetName) {
            auto world = Framework::CoreModules::GetWorldEngine()->GetWorld();

            auto weather             = world->get_mut<Core::Modules::Environment::Weather>();
            weather->_weatherSetName = weatherSetName;
            FW_SEND_COMPONENT_RPC(MafiaMP::Shared::RPC::SetEnvironment, SLNet::RakString(weather->_weatherSetName.c_str()), {});
        }

        static void Register(sol::state *luaEngine) {
            if (!luaEngine) {
                return;
            }

            sol::usertype<World> cls = luaEngine->new_usertype<World>("World");
            cls["createVehicle"]     = &World::CreateVehicle;
            cls["getDayTimeHours"]   = &World::GetDayTimeHours;
            cls["setDayTimeHours"]   = &World::SetDayTimeHours;
            cls["getWeatherSet"]     = &World::GetWeatherSet;
            cls["setWeatherSet"]     = &World::SetWeatherSet;
        }
    };
} // namespace MafiaMP::Scripting
