#pragma once

namespace ue::game::traffic {
    class C_TrafficSpawnManager {
        public:
            void SetTrainDensity(float);
            void SwitchAmbientTraffic(bool);


        static C_TrafficSpawnManager* GetInstance() {
            return *(C_TrafficSpawnManager**)0x0000001463DA8F0;
        }
    };
};