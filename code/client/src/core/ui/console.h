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
#include <cxxopts.hpp>
#include <spdlog/spdlog.h>

namespace MafiaMP::Core::UI {
    class Console final {
      public:
        using CommandProc = std::function<void(cxxopts::ParseResult &)>;
      private:
        struct CommandInfo {
            std::unique_ptr<cxxopts::Options> options;
            CommandProc proc;
        };
        bool _shouldDisplayWidget = true;
        bool _autoScroll          = true;
        bool _isOpen              = false;
        bool _focusOnConsole      = false;
        bool _focusInput          = false;
        bool _consoleControl      = false;
        std::vector<Game::Streaming::EntityTrackingInfo *> _TEMP_vehicles;
        std::unordered_map<std::string, CommandInfo> _commands;
        std::shared_ptr<Framework::Utils::States::Machine> _machine;
        spdlog::logger *_logger;
        void Disconnect();
        void DespawnAll();
        void SpawnCar();
        void CrashMe();
        void BreakMe();
        void CloseGame();
        void FormatLog(std::string log);
        void ProcessCommand(const std::string &);

      public:
        Console(std::shared_ptr<Framework::Utils::States::Machine> machine);
        ~Console() = default;

        void Toggle();
        bool Update();
        bool Open();
        bool Close();

        void RegisterCommand(const std::string &name, std::initializer_list<cxxopts::Option> options, const CommandProc &proc, const std::string &desc = "");
    };
} // namespace MafiaMP::Core::UI
