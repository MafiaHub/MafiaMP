#include "c_vehicle_real_light_manager.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::vehicle {
        void C_VehicleRealLightManager::SetLightIntensity(int light, float intensity) {
            hook::this_call<void>(gPatterns.C_VehicleRealLightManager__SetLightIntensity, this, light, intensity);
        }

        void C_VehicleRealLightManager::SetPlayerLights(bool state) {
            hook::this_call<void>(gPatterns.C_VehicleRealLightManager__SetPlayerLights, this, state);
        }

        void C_VehicleRealLightManager::SetReflectorLightsOn(bool state1, bool state2) {
            hook::this_call<void>(gPatterns.C_VehicleRealLightManager__SetReflectorLightsOn, this, state1, state2);
        }

        void C_VehicleRealLightManager::SetVisible(bool visible) {
            hook::this_call<void>(gPatterns.C_VehicleRealLightManager__SetVisible, this, visible);
        }
    }
}
