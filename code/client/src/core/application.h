#pragma once

#include <external/discord/wrapper.h>

namespace MafiaMP::Core {
    class Application {
      private:
        bool _initialized = false;
        Framework::External::Discord::Wrapper *_presence;

      public:
        Application();
        ~Application() {};

        bool Init();
        bool Shutdown();
        void Update();

        Framework::External::Discord::Wrapper* GetPresence() const {
            return _presence;
        }

        bool IsInitialized() const {
            return _initialized;
        }
    };

    extern std::unique_ptr<Application> gApplication;
}
