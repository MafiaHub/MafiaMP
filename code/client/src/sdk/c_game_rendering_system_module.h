#pragma once

#include "c_ticked_module.h"

#include "patterns.h"

namespace SDK {
    class C_GameRenderingSystemModule : public C_TickedModule {
      public:
        static C_GameRenderingSystemModule* GetGameRenderingSystemModule() {
            return hook::call<C_GameRenderingSystemModule*>(gPatterns.C_GameRenderingSystemModule__GetGameRenderingSystemModule);
        }
    };
}
