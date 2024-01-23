#include "c_weather_manager_2.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::gfx::environmenteffects {
        /**
         * When false, dynamic weather sets will not change if you change the time.
         * You should set SetDefaultTimeFlowSpeedMult or SetUserTimeFlowSpeedMult to 0 to disable time flow.
         */
        void C_WeatherManager2::EnableTimeFlow(bool enable) {
            return hook::this_call<void>(gPatterns.C_WeatherManager2__EnableTimeFlow, this, enable);
        }

        float C_WeatherManager2::GetDayTimeHours() {
            return hook::this_call<float>(gPatterns.C_WeatherManager2__GetDayTimeHours, this);
        }

        float C_WeatherManager2::GetDayTimeRel() {
            return hook::this_call<float>(gPatterns.C_WeatherManager2__GetDayTimeRel, this);
        }

        float C_WeatherManager2::GetDefaultTimeFlowSpeedMult() {
            return hook::this_call<float>(gPatterns.C_WeatherManager2__GetDefaultTimeFlowSpeedMult, this);
        }

        float C_WeatherManager2::GetUserTimeFlowSpeedMult() {
            return hook::this_call<float>(gPatterns.C_WeatherManager2__GetUserTimeFlowSpeedMult, this);
        }

        bool C_WeatherManager2::IsTimeFlowEnabled() {
            return hook::this_call<bool>(gPatterns.C_WeatherManager2__IsTimeFlowEnabled, this);
        }

        void C_WeatherManager2::SetDayTimeHours(float hour) {
            hook::this_call<void>(gPatterns.C_WeatherManager2__SetDayTimeHours, this, hour);
        }

        void C_WeatherManager2::SetDayTimeSec(float seconds) {
            hook::this_call(gPatterns.C_WeatherManager2__SetDayTimeSec, this, seconds);
        }

        void C_WeatherManager2::SetDefaultTimeFlowSpeedMult(float speedMult) {
            return hook::this_call<void>(gPatterns.C_WeatherManager2__SetDefaultTimeFlowSpeedMult, this, speedMult);
        }

        void C_WeatherManager2::SetUserTimeFlowSpeedMult(float speedMult) {
            return hook::this_call<void>(gPatterns.C_WeatherManager2__SetUserTimeFlowSpeedMult, this, speedMult);
        }

        /**
         * @param weatherSetName Name of the weather set
         * @param transitionSpeed Speed ​​at which we move from one weather to another. 0 for immediately.
         */
        void C_WeatherManager2::SetWeatherSet(ue::C_String const &weatherSetName, float transitionSpeed) {
            hook::this_call(gPatterns.C_WeatherManager2__SetWeatherSet, this, weatherSetName, transitionSpeed);
        }
    } // namespace ue::gfx::environmenteffects
} // namespace SDK
