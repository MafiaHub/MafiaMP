#pragma once

#include <utils/safe_win32.h>
#include <memory>

#include "ui/console.h"

#include <integrations/client/instance.h>

#include <utils/states/machine.h>

namespace MafiaMP::Core {
    class Application: public Framework::Integrations::Client::Instance {
      private:
        std::shared_ptr<Framework::Utils::States::Machine> _stateMachine;
        std::shared_ptr<UI::Console> _console;
        std::shared_ptr<Game::Streaming::EntityFactory> _entityFactory;
        flecs::entity _localPlayer;
        float _tickInterval = 0.01667f;

      public:
        virtual bool PostInit() override;
        virtual bool PreShutdown() override;
        virtual void PostUpdate() override;

        void InitNetworkingMessages();

        std::shared_ptr<Framework::Utils::States::Machine> GetStateMachine() const {
            return _stateMachine;
        }

        std::shared_ptr<Game::Streaming::EntityFactory> GetEntityFactory() const {
            return _entityFactory;
        }

        float GetTickInterval() const {
            return _tickInterval;
        }

        std::shared_ptr<UI::Console> GetDevConsole() const {
            return _console;
        }
    };

    extern std::unique_ptr<Application> gApplication;
} // namespace MafiaMP::Core
