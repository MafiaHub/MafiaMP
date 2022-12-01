#pragma once

#include <string>
#include <flecs/flecs.h>

namespace MafiaMP::Core::Modules {
    class Environment {
      public:
          struct Weather {
            std::string _weatherSetName;
            float _dayTimeHours;
          };

          Environment(flecs::world &world);
    };
}
