#pragma once

#include <utils/safe_win32.h>

#include "../game/streaming/entity_factory.h"

#include <integrations/client/instance.h>

#include <utils/states/machine.h>

namespace MafiaMP::Core {
    struct CurrentState {
        std::string _host;
        int32_t _port;  
        std::string _nickname;
    };

    class Application: public Framework::Integrations::Client::Instance {
      private:
        Framework::Utils::States::Machine *_stateMachine;
        Game::Streaming::EntityFactory *_entityFactory;

        CurrentState _currentState;

      public:
        virtual bool PostInit() override;
        virtual bool PreShutdown() override;
        virtual void PostUpdate() override;

        Framework::Utils::States::Machine* GetStateMachine() const {
            return _stateMachine;
        }

        Game::Streaming::EntityFactory *GetEntityFactory() const {
            return _entityFactory;
        }

        CurrentState GetCurrentState() const {
            return _currentState;
        }

        void SetCurrentState(CurrentState state) {
            _currentState = state;
        }
    };

    extern std::unique_ptr<Application> gApplication;
} // namespace MafiaMP::Core
