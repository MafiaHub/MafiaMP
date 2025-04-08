#include "c_traffic_spawn_manager.h"

namespace SDK {
    namespace ue::game::traffic {
        float C_TrafficSpawnManager::GetCivilCarDensityMult() {
            return hook::this_call<float>(gPatterns.C_TrafficSpawnManager__GetCivilCarDensityMult, this);
        }

        void C_TrafficSpawnManager::Populate(bool arg1) {
            hook::this_call<void>(gPatterns.C_TrafficSpawnManager__Populate, this, arg1);
        }

        C_SpawnPointsManager *C_TrafficSpawnManager::SetTrainDensity(float density) {
            return hook::this_call<C_SpawnPointsManager *>(gPatterns.C_TrafficSpawnManager__SetTrainDensity, this, density);
        }

        bool C_TrafficSpawnManager::SwitchAmbientTraffic(bool enable) {
            return hook::this_call<bool>(gPatterns.C_TrafficSpawnManager__SwitchAmbientTraffic, this, enable);
        }

        int C_TrafficSpawnManager::UpdateMaxElementsCnt() {
            return hook::this_call<int>(gPatterns.C_TrafficSpawnManager__UpdateMaxElementsCnt, this);
        }
    } // namespace ue::game::traffic
} // namespace SDK
