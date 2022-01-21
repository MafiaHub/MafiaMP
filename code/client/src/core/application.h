#pragma once

#include <utils/safe_win32.h>
#include <memory>

#include "ui/console.h"

#include <integrations/client/instance.h>

#include <utils/states/machine.h>

namespace MafiaMP::Core {
    class Application: public Framework::Integrations::Client::Instance {
      private:
        Framework::Utils::States::Machine *_stateMachine;
        std::unique_ptr<UI::Console> _console;
        Game::Streaming::EntityFactory *_entityFactory;
        flecs::entity _localPlayer;

      public:
        virtual bool PostInit() override;
        virtual bool PreShutdown() override;
        virtual void PostUpdate() override;

        void InitNetworkingMessages();

        Framework::Utils::States::Machine* GetStateMachine() const {
            return _stateMachine;
        }

        Game::Streaming::EntityFactory *GetEntityFactory() const {
            return _entityFactory;
        }
    };

    extern std::unique_ptr<Application> gApplication;
} // namespace MafiaMP::Core
