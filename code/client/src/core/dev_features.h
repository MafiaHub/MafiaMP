#pragma once

#include <memory>
#include <string>

#include "game/streaming/entity_tracking_info.h"

#include "ui/devs/camera_studio.h"
#include "ui/devs/debug_audio.h"
#include "ui/devs/debug_player.h"
#include "ui/devs/debug_vehicle.h"
#include "ui/devs/debug_world.h"
#include "ui/devs/entity_browser.h"
#include "ui/devs/network_stats.h"

namespace MafiaMP::Core {
    class DevFeatures final {
      private:
        Game::Streaming::EntityTrackingInfo *_TEMP_HUMAN = nullptr;

        std::shared_ptr<UI::Devs::CameraStudio> _cameraStudio {};

        std::shared_ptr<UI::Devs::DebugAudio> _debugAudio {};

        std::shared_ptr<UI::Devs::DebugPlayer> _debugPlayer {};

        std::shared_ptr<UI::Devs::DebugVehicle> _debugVehicle {};

        std::shared_ptr<UI::Devs::DebugWorld> _debugWorld {};

        std::shared_ptr<UI::Devs::EntityBrowser> _entityBrowser {};

        std::shared_ptr<UI::Devs::NetworkStats> _networkStats {};

      public:
        DevFeatures();
        void Init();
        void Update();
        void Shutdown();

      private:
        void SetupCommands();
        void SetupMenuBar();

        void ToggleCameraStudio();
        void ToggleDebugAudio();
        void ToggleDebugPlayer();
        void ToggleDebugVehicle();
        void ToggleDebugWorld();
        void ToggleEntityBrowser();
        void ToggleNetworkStats();

        void Disconnect();
        void CrashMe();
        void BreakMe();
        void CloseGame();
        void SpawnCrashObject();
    };
} // namespace MafiaMP::Core
