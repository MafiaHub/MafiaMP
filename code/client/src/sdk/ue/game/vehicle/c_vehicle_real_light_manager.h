#pragma once

namespace SDK {
    namespace ue::game::vehicle {
        class C_VehicleRealLightManager {
          public:
            void SetLightIntensity(int, float);
            void SetPlayerLights(bool);
            void SetReflectorLightsOn(bool, bool);
            void SetVisible(bool);
        };
    }
}
