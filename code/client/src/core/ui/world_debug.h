#pragma once

#include "utils/safe_win32.h"

#include <string>
#include <vector>

#include "game/streaming/entity_tracking_info.h"

namespace MafiaMP::Core::UI {
    class WorldDebug final {
      public:
        WorldDebug();

        void Update();

        bool IsVisible() const {
            return _visible;
        }

        void SetVisible(bool visible) {
            _visible = visible;
        }

        void SpawnCar(std::string modelName = "berkley_810", bool putPlayerIn = false, bool replacePrevious = false);

      private:
        bool _visible = false;

        std::vector<Game::Streaming::EntityTrackingInfo *> _spawnedVehicles;
    };
} // namespace MafiaMP::Core::UI
