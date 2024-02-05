#include "c_traffic_spawn_manager.h"

namespace SDK {
    namespace ue::game::traffic {
        float C_TrafficSpawnManager::GetCivilCarDensityMult() {
            return hook::this_call<float>(reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC 20 45 33 C0 33 D2 48 8B D9 E8 ? ? ? ? 48 8D 0D ? ? ? ?")), this);
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
            return hook::this_call<int>(hook::get_opcode_address("E9 ? ? ? ? 48 83 C4 28 C3 CC CC CC 88 51 50"), this);
        }
    } // namespace ue::game::traffic
} // namespace SDK
