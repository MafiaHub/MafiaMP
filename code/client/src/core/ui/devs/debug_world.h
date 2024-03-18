#pragma once

#include "../ui_base.h"

#include <string>
#include <vector>

#include "game/streaming/entity_tracking_info.h"

namespace MafiaMP::Core::UI::Devs {
    class DebugWorld final: public UIBase {
      private:
        std::vector<Game::Streaming::EntityTrackingInfo *> _spawnedVehicles;

        virtual void OnOpen() override;

        virtual void OnClose() override;

        virtual void OnUpdate() override;

      public:
        DebugWorld(): UIBase() {};

        void SpawnCar(std::string modelName = "berkley_810", bool putPlayerIn = false, bool replacePrevious = false);
    };
} // namespace MafiaMP::Core::UI::Devs
