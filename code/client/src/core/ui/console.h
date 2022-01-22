#pragma once

#include <utils/safe_win32.h>

#include "../../game/streaming/entity_tracking_info.h"
#include "../../game/streaming/entity_factory.h"

#include <utils/states/state.h>
#include <integrations/client/instance.h>
#include <utils/states/machine.h>

#include <memory>

namespace MafiaMP::Core::UI {
    class Console {
      private:
        bool _shouldDisplayWidget = false;
        bool _isOpen = false;
        bool _autoScroll = false;
        std::vector<Game::Streaming::EntityTrackingInfo *> _TEMP_vehicles;
        std::shared_ptr<Framework::Utils::States::Machine> _machine;
        void Disconnect();
        void DespawnAll();
        void SpawnCar();

      public:
        Console(std::shared_ptr<Framework::Utils::States::Machine>);
        ~Console();

        void Toggle();
        bool Init();
        bool Update();
        bool Open();
        bool Close();
    };
} // namespace MafiaMP::Core::Modules
