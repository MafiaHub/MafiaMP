/*
 * MafiaHub OSS license
 * Copyright (c) 2022, MafiaHub. All rights reserved.
 *
 * This file comes from MafiaHub, hosted at https://github.com/MafiaHub/Framework.
 * See LICENSE file in the source repository for information regarding licensing.
 */

#pragma once

#include "game/streaming/entity_tracking_info.h"

#include <memory>
#include <string>
#include <vector>

#include "ui/audio_debug.h"
#include "ui/camera_studio.h"
#include "ui/entity_browser.h"
#include "ui/network_stats.h"
#include "ui/player_debug.h"
#include "ui/vehicle_debug.h"

#include "sdk/entities/c_human_2.h"

namespace MafiaMP::Core {
    class DevFeatures final {
      private:
        std::vector<Game::Streaming::EntityTrackingInfo *> _TEMP_vehicles;
        Game::Streaming::EntityTrackingInfo *_TEMP_HUMAN = nullptr;

        std::shared_ptr<UI::EntityBrowser> _entityBrowser {};

        std::shared_ptr<UI::CameraStudio> _cameraStudio {};

        std::shared_ptr<UI::AudioDebug> _audioDebug {};

        std::shared_ptr<UI::PlayerDebug> _playerDebug {};

        std::shared_ptr<UI::VehicleDebug> _vehicleDebug {};

        std::shared_ptr<UI::NetworkStats> _networkStats {};

      public:
        DevFeatures();
        void Init();
        void Update();
        void Shutdown();

      private:
        void SetupCommands();
        void SetupMenuBar();
        void Disconnect();
        void DespawnAll();
        void SpawnCrashObject();
        void SpawnCar(std::string modelName = "shubert_e_six_p");
        void SpawnRandomCar();
        void CrashMe();
        void BreakMe();
        void CloseGame();

        void ToggleEntityBrowser();
        void ToggleCameraStudio();
        void ToggleAudioDebug();
        void TogglePlayerDebug();
        void ToggleVehicleDebug();
        void ToggleNetworkStats();
    };
} // namespace MafiaMP::Core
