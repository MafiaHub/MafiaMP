#include "application.h"

#include <logging/logger.h>

namespace MafiaMP::Core {
    std::unique_ptr<Application> gApplication = nullptr;

    Application::Application() {
        _presence = new Framework::External::Discord::Wrapper;
        _renderer = new Framework::GUI::Renderer;
    }

    bool Application::Init() {
        if (_presence) {
            _presence->Init(763114144454672444);
        }

        if (_renderer) {
            Framework::GUI::RendererConfiguration config;
            config.backend = Framework::GUI::RendererBackend::BACKEND_D3D_11;
            config.imgui   = true;
            _renderer->Init(config);
        }

        Framework::Logging::GetLogger("Application")->debug("Initialize success");
        _initialized = true;
        return true;
    }

    bool Application::Shutdown() {
        if (_renderer) {
            _renderer->Shutdown();
        }
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
