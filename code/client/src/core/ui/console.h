#pragma once

#include <utils/safe_win32.h>

#include "../../game/streaming/entity_factory.h"
#include "../../game/streaming/entity_tracking_info.h"

#include <integrations/client/instance.h>
#include <utils/states/machine.h>
#include <utils/states/state.h>
#include <utils/command_processor.h>

#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <cxxopts.hpp>
#include <spdlog/spdlog.h>

namespace MafiaMP::Core::UI {
    class Console final {
      public:
        using MenuBarProc = std::function<void()>;
      private:
        bool _shouldDisplayWidget = true;
        bool _autoScroll          = true;
        bool _isOpen              = false;
        bool _focusOnConsole      = false;
        bool _focusInput          = false;
        bool _consoleControl      = false;
        std::vector<Game::Streaming::EntityTrackingInfo *> _TEMP_vehicles;
        std::shared_ptr<Framework::Utils::CommandProcessor> _commandProcessor;
        std::vector<MenuBarProc> _menuBarDrawers;
        spdlog::logger *_logger;
        void FormatLog(std::string log);
        void SendCommand(const std::string &command);

      public:
        Console(std::shared_ptr<Framework::Utils::CommandProcessor> commandProcessor);
        ~Console() = default;

        void Toggle();
        bool Update();
        bool Open();
        bool Close();

        void RegisterMenuBarDrawer(MenuBarProc proc) {
            _menuBarDrawers.push_back(proc);
        }

        bool IsOpen() const {
            return _isOpen;
        }
    };
} // namespace MafiaMP::Core::UI
