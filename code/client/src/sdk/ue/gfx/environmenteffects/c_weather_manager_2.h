#pragma once

#include "../../c_string.h"

namespace SDK {
    namespace ue::gfx::environmenteffects {
        class C_WeatherManager2 {
          public:
            float GetDayTimeHours();
            float GetDayTimeRel();

            void SetDayTimeHours(float);
            void SetDayTimeRel(float);
            void SetDayTimeSec(float);
            void SetWeatherSet(ue::C_String const &, float);
        };
    } // namespace ue::gfx::environmenteffects
}
