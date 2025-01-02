#pragma once

#include "sdk/ue/c_string.h"

namespace SDK {
    namespace ue::gfx::environmenteffects {
        class C_WeatherManager2 {
          public:
            void EnableTimeFlow(bool);
            float GetDayTimeHours();
            float GetDayTimeRel();
            float GetDefaultTimeFlowSpeedMult();
            float GetUserTimeFlowSpeedMult();
            bool IsTimeFlowEnabled();
            void SetDayTimeHours(float);
            void SetDayTimeSec(float);
            void SetDefaultTimeFlowSpeedMult(float);
            void SetUserTimeFlowSpeedMult(float);
            void SetWeatherSet(ue::C_String const &, float);
        };
    } // namespace ue::gfx::environmenteffects
} // namespace SDK
