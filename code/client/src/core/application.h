#pragma once

#include "../game/streaming/entity_factory.h"

#include <integrations/client/instance.h>

#include <utils/states/machine.h>

namespace MafiaMP::Core {
    class Application: public Framework::Integrations::Client::Instance {
      private:
        Framework::Utils::States::Machine *_stateMachine;
        Game::Streaming::EntityFactory *_entityFactory;

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
    };

    extern std::unique_ptr<Application> gApplication;
} // namespace MafiaMP::Core
