#pragma once

#include "utils/safe_win32.h"

namespace MafiaMP::Core::UI {
    class NetworkStats final {
      public:
        NetworkStats();

        void Update();
    };
} // namespace MafiaMP::Core::UI
