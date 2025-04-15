#pragma once

#include "c_weather_manager_2.h"
#include "sdk/ue/sys/core/c_object_spawner.h"

namespace SDK {
    namespace ue::gfx::environmenteffects {
        class C_GfxEnvironmentEffects {
          protected:
            char pad0[0x10];                                    // 0000 - 0010
            int m_iDecal;                                       // 0010 - 0014
            char pad1[0x14];                                    // 0014 - 0028
            C_WeatherManager2 *m_pWeatherManager;               // 0028 - 0030
            char pad2[0x80];                                    // 0030 - 00B0
            ue::sys::core::C_ObjectSpawner *m_pObjectSpawner;   // 00B0 - 00B8

          public:
            int GetDecal() const;
            C_WeatherManager2 *GetWeatherManager() const {
                return m_pWeatherManager;
            }
            static C_GfxEnvironmentEffects *GetInstance();
        };
    } // namespace ue::gfx::environmenteffects
}
