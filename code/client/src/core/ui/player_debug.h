#pragma once

#include "utils/safe_win32.h"

#include <fu2/function2.hpp>

#include <string>
#include <unordered_map>
#include <vector>

#include "sdk/entities/c_entity.h"

namespace MafiaMP::Core::UI {
    class PlayerDebug final {
      public:
        PlayerDebug();

        void Update(bool *isVisible);
    };
} // namespace MafiaMP::Core::UI
