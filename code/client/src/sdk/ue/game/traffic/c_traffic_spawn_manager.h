#pragma once

#include "../../../patterns.h"

#include "c_spawn_point_manager.h"

namespace SDK {
    namespace ue::game::traffic {
        class C_TrafficSpawnManager {
          public:
            float GetCivilCarDensityMult();

            void Populate(bool arg1); // TODO: find returns

            C_SpawnPointsManager *SetTrainDensity(float density);

            bool SwitchAmbientTraffic(bool enable);

            /**
             * @returns unknown, value of C_StreamingTrafficModule::GetTrafficSettings[12] and also this + 11
             */
            int UpdateMaxElementsCnt();

            static C_TrafficSpawnManager *GetInstance() {
                // TODO: switch to pattern - find the path to the pointer
                return *reinterpret_cast<C_TrafficSpawnManager **>(0x1463DDB10);
                // return hook::this_call<C_TrafficSpawnManager *>(gPatterns.C_TrafficSpawnManager__Instance);
            }
        };
    } // namespace ue::game::traffic
} // namespace SDK
