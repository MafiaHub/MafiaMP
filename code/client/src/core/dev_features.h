/*
 * MafiaHub OSS license
 * Copyright (c) 2022, MafiaHub. All rights reserved.
 *
 * This file comes from MafiaHub, hosted at https://github.com/MafiaHub/Framework.
 * See LICENSE file in the source repository for information regarding licensing.
 */

#pragma once

#include "game/streaming/entity_tracking_info.h"
#include <vector>
namespace MafiaMP::Core {
    class DevFeatures final {
      private:
        std::vector<Game::Streaming::EntityTrackingInfo *> _TEMP_vehicles;

      public:
        void Init();
        void Update();
        void Shutdown();

      private:
        void SetupCommands();
        void SetupMenuBar();
        void Disconnect();
        void DespawnAll();
        void SpawnCar(std::string modelName = "berkley_810");
        void CrashMe();
        void BreakMe();
        void CloseGame();
    };
} // namespace MafiaMP::Core
