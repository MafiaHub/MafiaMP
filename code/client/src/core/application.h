#pragma once

#include <memory>

#include <utils/safe_win32.h>

#include "../game/helpers/districts.h"
#include "../game/streaming/entity_factory.h"

#include "luavm.h"
#include "ui/chat.h"
#include "ui/console.h"

#include "dev_features.h"

#include "game/helpers/game_input.h"

#include <integrations/client/instance.h>
#include <utils/command_processor.h>
#include <utils/states/machine.h>
#include <gui/manager.h>

namespace MafiaMP::Core {
    class Application: public Framework::Integrations::Client::Instance {
      private:
        std::shared_ptr<Framework::Utils::States::Machine> _stateMachine;
        std::shared_ptr<UI::Console> _console;
        std::shared_ptr<UI::Chat> _chat;
        std::shared_ptr<Framework::GUI::Manager> _webManager;
        std::shared_ptr<Game::Streaming::EntityFactory> _entityFactory;
        std::shared_ptr<Framework::Utils::CommandProcessor> _commandProcessor;
        std::shared_ptr<MafiaMP::Game::GameInput> _input;
        std::shared_ptr<LuaVM> _luaVM;
        flecs::entity _localPlayer;
        DevFeatures _devFeatures;

        float _tickInterval = 0.01667f;

        int _lockControlsCounter   = 0;
        bool _lockControlsBypassed = false;

        int _mainMenuViewId = -1;

      private:
        Game::Helpers::Districts _lastDistrictID = Game::Helpers::Districts::UNSPECIFIED;

        void ProcessLockControls(bool lock);

      public:
        bool PostInit() override;
        bool PreShutdown() override;
        void PostUpdate() override;
        void PostRender() override;

        void InitNetworkingMessages();
        void InitRPCs();

        void PimpMyImGUI();

        void LockControls(bool lock);
        bool AreControlsLocked() const {
            return _lockControlsCounter > 0;
        }
        void ToggleLockControlsBypass();
        bool AreControlsLockedBypassed() const {
            return _lockControlsBypassed;
        }

        std::shared_ptr<Framework::Utils::States::Machine> GetStateMachine() const {
            return _stateMachine;
        }

        std::shared_ptr<Game::Streaming::EntityFactory> GetEntityFactory() const {
            return _entityFactory;
        }

        float GetTickInterval() const {
            return _tickInterval;
        }

        std::shared_ptr<Framework::Utils::CommandProcessor> GetCommandProcessor() const {
            return _commandProcessor;
        }

        std::shared_ptr<MafiaMP::Game::GameInput> GetInput() const {
            return _input;
        }

        std::shared_ptr<UI::Console> GetConsole() const {
            return _console;
        }

        std::shared_ptr<UI::Chat> GetChat() const {
            return _chat;
        }

        std::shared_ptr<LuaVM> GetLuaVM() const {
            return _luaVM;
        }

        std::shared_ptr<Framework::GUI::Manager> GetWebManager() const {
            return _webManager;
        }

        flecs::entity GetLocalPlayer() const {
            return _localPlayer;
        }

        uint64_t GetLocalPlayerID();
        uint64_t GetLocalPlayerOwnerID();

        Game::Helpers::Districts GetLastDistrictID() const {
            return _lastDistrictID;
        }

        void SetLastDistrictID(Game::Helpers::Districts id) {
            _lastDistrictID = id;
        }

        int GetMainMenuViewId() const {
            return _mainMenuViewId;
        }
    };

    extern std::unique_ptr<Application> gApplication;
    extern std::string gProjectPath;
} // namespace MafiaMP::Core
