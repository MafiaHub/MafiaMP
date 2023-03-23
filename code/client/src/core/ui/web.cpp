#include "web.h"

#include <AppCore/Platform.h>

namespace MafiaMP::Core::UI {
    bool Web::Init() {
        // Prepare the configuration object
        ultralight::Config config;
        config.resource_path = "./resources/";
        config.use_gpu_renderer = false;
        config.device_scale = 1.0;

        // Configure the platform via the singleton
        ultralight::Platform::instance().set_config(config);
        ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());
        ultralight::Platform::instance().set_file_system(ultralight::GetPlatformFileSystem("."));
        ultralight::Platform::instance().set_logger(ultralight::GetDefaultLogger("ultralight.log"));

        // Create our renderer
        _renderer = ultralight::Renderer::Create();
        if (!_renderer || !_renderer.get()) {
            return false;
        }

        return true;
    }

    bool Web::CreateView(std::string name, int width, int height, std::string url) {
        ultralight::RefPtr<ultralight::View> view = _renderer->CreateView(width, height, false, nullptr);
        if (!view || !view.get()) {
            return false;
        }

        view->LoadURL(ultralight::String(url.c_str()));
        view->Focus();

        _views.emplace(name, view);
        return true;
    }

    bool Web::ToggleViewFocus(std::string name) {
        const auto view = _views[name];
        if (!view || !view.get()) {
            return false;
        }

        if (view->HasFocus()) {
            view->Unfocus();
        }
        else {
            view->Focus();
        }
        return true;
    }

    void Web::Update() {
        if (!_renderer || !_renderer.get()) {
            return;
        }

        _renderer->Update();
    }

    void Web::Render() {
        if (!_renderer || !_renderer.get()) {
            return;
        }

        _renderer->Render();
    }
}
