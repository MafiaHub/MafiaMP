#pragma once

#include "../game/streaming/entity_factory.h"

#include <integrations/client/instance.h>

namespace MafiaMP::Core {
    class Application: public Framework::Integrations::Client::Instance {
      private:
        Game::Streaming::EntityFactory *_entityFactory;

      public:
        virtual bool PostInit() override;
        virtual bool PreShutdown() override;
        virtual void PostUpdate() override;

        Game::Streaming::EntityFactory *GetEntityFactory() const {
            return _entityFactory;
        }
    };

    extern std::unique_ptr<Application> gApplication;
} // namespace MafiaMP::Core
