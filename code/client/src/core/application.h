#pragma once

#include <gui/renderer.h>
#include <external/discord/wrapper.h>

namespace MafiaMP::Core {
    class Application {
      private:
        bool _initialized = false;
        Framework::External::Discord::Wrapper *_presence;
        Framework::GUI::Renderer *_renderer;

      public:
        Application();
        ~Application() {};

        bool Init();
        bool Shutdown();

        void Update();
        void Render();

        Framework::External::Discord::Wrapper* GetPresence() const {
            return _presence;
        }

        Framework::GUI::Renderer* GetRenderer() const {
            return _renderer;
        }

        bool IsInitialized() const {
            return _initialized;
        }
    };

    extern std::unique_ptr<Application> gApplication;
}
