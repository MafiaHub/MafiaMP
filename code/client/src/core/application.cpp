#include "application.h"

#include <logging/logger.h>

namespace MafiaMP::Core {
    std::unique_ptr<Application> gApplication = nullptr;

    Application::Application() {
        _presence = new Framework::External::Discord::Wrapper;
    }

    bool Application::Init() {
        if (_presence) {
            _presence->Init(763114144454672444);
        }

        Framework::Logging::GetLogger("Application")->debug("Initialize success");
        _initialized = true;
        return true;
    }

    bool Application::Shutdown() {
        return true;
    }

    void Application::Update() {
        if (_presence) {
            _presence->Update();
        }
    }

    void Application::Render() {

    }
} // namespace MafiaMP::Core
