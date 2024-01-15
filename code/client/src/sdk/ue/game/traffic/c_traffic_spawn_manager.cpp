#include "c_traffic_spawn_manager.h"

#include "../../../patterns.h"

#include <utils/hooking/hooking.h>

namespace ue::game::traffic {
    void C_TrafficSpawnManager::SetTrainDensity(float density) {
        hook::this_call<void>(gPatterns.C_TrafficSpawnManager__SetTrainDensity, this, density);
    }

    void C_TrafficSpawnManager::SwitchAmbientTraffic(bool enable) {
        // TODO: switch to pattern - find the path to the pointer
        hook::this_call<void>(0x000000142F0C, this, enable);
    }
};