#include "application.h"

#include <logging/logger.h>

namespace MafiaMP::Core {
    std::unique_ptr<Application> gApplication = nullptr;

    bool Application::PostInit() {
        _entityFactory = new Game::Streaming::EntityFactory;
        return true;
    }

    bool Application::PreShutdown() {
        if (_entityFactory) {
            _entityFactory->ReturnAll();
        }
        return true;
    }

    void Application::PostUpdate() {
        if (_entityFactory) {
            _entityFactory->Update();
        }
    }
} // namespace MafiaMP::Core
