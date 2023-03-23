#pragma once

#include <ultralight/Ultralight.h>

#include <string>
#include <unordered_map>

namespace MafiaMP::Core::UI {
    class Web {
      protected:
        ultralight::RefPtr<ultralight::Renderer> _renderer;
        std::unordered_map<std::string, ultralight::RefPtr<ultralight::View>> _views;

        public:
          bool Init();
          bool CreateView(std::string name, int width, int height, std::string url);
          bool ToggleViewFocus(std::string name);

          void Update();
          void Render();
    };
}
