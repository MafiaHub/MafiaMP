#pragma once

#include <utils/safe_win32.h>

#include "../../game/streaming/entity_tracking_info.h"
#include "../../game/streaming/entity_factory.h"

#include <utils/states/state.h>
#include <integrations/client/instance.h>
#include <utils/states/machine.h>

namespace MafiaMP::Core::UI {
    class Console {
      private:
        bool _shouldDisplayWidget = false;
        bool _isOpen = false;
        bool _autoScroll = false;
        std::vector<Game::Streaming::EntityTrackingInfo *> _TEMP_vehicles;
        Framework::Utils::States::Machine *_machine;
        void Disconnect();
        void DespawnAll();
        void SpawnCar();

      public:
        Console(Framework::Utils::States::Machine *m);
        ~Console();

        void Toggle();
        bool Init();
        bool Update();
        bool Open();
        bool Close();
    };
} // namespace MafiaMP::Core::Modules
