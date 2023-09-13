#pragma once
#include <utils/safe_win32.h>

namespace MafiaMP::Core::UI {
    class Web {
      public:
        bool Init();

        void Update();
        void Render();
    };
}
