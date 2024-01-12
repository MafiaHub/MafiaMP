#pragma once

#include "utils/safe_win32.h"
#include <utils/time.h>

namespace MafiaMP::Core::UI {
    class NetworkStats final {
      public:
        NetworkStats();

        void Update(bool *isVisible);

      private:
        int64_t _nextStatsUpdate = 0;
        char _stats[8192]        = {0};
    };
} // namespace MafiaMP::Core::UI
