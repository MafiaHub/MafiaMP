#pragma once

#include <utils/safe_win32.h>

#include "../../game/streaming/entity_factory.h"
#include "../../game/streaming/entity_tracking_info.h"

#include <integrations/client/instance.h>
#include <utils/states/machine.h>
#include <utils/states/state.h>

#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

namespace MafiaMP::Core::UI {
    class Console final {
      public:
        using CommandProc = std::function<void(const std::string &, const std::vector<std::string> &)>;
      private:
        bool _shouldDisplayWidget = true;
        bool _autoScroll          = true;
        bool _isOpen              = false;
        bool _focusOnConsole      = false;
        std::vector<Game::Streaming::EntityTrackingInfo *> _TEMP_vehicles;
        std::unordered_map<std::string, CommandProc> _commands;
        std::shared_ptr<Framework::Utils::States::Machine> _machine;
        void Disconnect();
        void DespawnAll();
        void SpawnCar();
        void CrashMe();
        void BreakMe();
        void FormatLog(std::string log);
        void ProcessCommand(const std::string &);

      public:
        Console(std::shared_ptr<Framework::Utils::States::Machine> machine);
        ~Console() = default;

        void Toggle();
        bool Update();
        bool Open();
        bool Close();

        void RegisterCommand(const std::string &name, const CommandProc &proc);
    };
} // namespace MafiaMP::Core::UI
