#pragma once

#include "patterns.h"

namespace SDK {
    class C_GameGfxEnvEffModule {
      public:
        static const char *GetCurrentWeatherSetName() {
            return hook::call<const char *>(gPatterns.C_GameGfxEnvEffModule__GetCurrentWeatherSetName);
        }
    };
} // namespace SDK
