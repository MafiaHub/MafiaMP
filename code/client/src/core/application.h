#pragma once

#include <memory>

#include <utils/safe_win32.h>

#include "luavm.h"
#include "../game/streaming/entity_factory.h"
#include "ui/console.h"
#include "ui/chat.h"
#include "dev_features.h"

#include <integrations/client/instance.h>
#include <utils/states/machine.h>
#include <utils/command_processor.h>

namespace MafiaMP::Core {
    class Application: public Framework::Integrations::Client::Instance {
      private:
        friend class DevFeatures;
        std::shared_ptr<Framework::Utils::States::Machine> _stateMachine;
        std::shared_ptr<UI::MafiaConsole> _console;
        std::shared_ptr<UI::Chat> _chat;
        std::shared_ptr<Game::Streaming::EntityFactory> _entityFactory;
        std::shared_ptr<Framework::Utils::CommandProcessor> _commandProcessor;
        std::shared_ptr<LuaVM> _luaVM;
        flecs::entity _localPlayer;
        DevFeatures _devFeatures;
        float _tickInterval = 0.01667f;

      public:
        bool PostInit() override;
        bool PreShutdown() override;
        void PostUpdate() override;

        void InitNetworkingMessages();
        void InitRPCs();

        void PimpMyImGUI();
        void LockControls(bool lock);
        
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

        std::shared_ptr<UI::MafiaConsole> GetDevConsole() const {
            return _console;
        }

        std::shared_ptr<UI::Chat> GetChat() const {
            return _chat;
        }

        flecs::entity GetLocalPlayer() const {
            return _localPlayer;
        }
        
        uint64_t GetLocalPlayerID();
    };

    extern std::unique_ptr<Application> gApplication;
    extern std::string gProjectPath;
} // namespace MafiaMP::Core
