#include "c_traffic_spawn_manager.h"

#include <utils/hooking/hooking.h>

namespace ue::game::traffic {
    void C_TrafficSpawnManager::SetTrainDensity(float density) {
        // TODO: switch to pattern
        hook::this_call<void>(0x000000142F05430, this, density);
    }

    void C_TrafficSpawnManager::SwitchAmbientTraffic(bool enable) {
        // TODO: switch to pattern
        hook::this_call<void>(0x000000142F0C, this, enable);
    }
};