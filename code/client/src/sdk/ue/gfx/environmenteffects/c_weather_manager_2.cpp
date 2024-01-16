#include "c_weather_manager_2.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::gfx::environmenteffects {
        float C_WeatherManager2::GetDayTimeHours() {
            return hook::this_call<float>(gPatterns.C_WeatherManager2__GetDayTimeHours, this);
        }

        float C_WeatherManager2::GetDayTimeRel() {
            return hook::this_call<float>(gPatterns.C_WeatherManager2__GetDayTimeRel, this);
        }

        void C_WeatherManager2::SetDayTimeHours(float time) {
            hook::this_call<float>(gPatterns.C_WeatherManager2__SetDayTimeHours, this, time);
        }

        void C_WeatherManager2::SetDayTimeRel(float time) {
            hook::this_call<float>(gPatterns.C_WeatherManager2__SetDayTimeRel, this, time);
        }

        void C_WeatherManager2::SetDayTimeSec(float time) {
            // const auto C_WeatherManager2__SetDayTimeSecAddr = reinterpret_cast<uint64_t>(hook::pattern("").first());
            // hook::this_call<float>(0x0000001421076A0, this, time);
        }

        void C_WeatherManager2::SetWeatherSet(ue::C_String const &preset, float time) {
            hook::this_call(gPatterns.C_WeatherManager2__SetWeatherSet, this, preset, time);
        }
    } // namespace ue::gfx::environmenteffects
} // namespace SDK
