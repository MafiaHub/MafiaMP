#pragma once

#include "../sdk/c_ticked_module.h"
#include "../sdk/c_ticked_module_manager.h"

#include <functional>

namespace MafiaMP::Game {
    class Module: public SDK::C_TickedModule {
      public:
        Module();

        void OnGameInit(SDK::I_TickedModuleCallEventContext &);
        void OnGameShutdown(SDK::I_TickedModuleCallEventContext &);
        void OnGameRender(SDK::I_TickedModuleCallEventContext &);
        void OnGameTick(SDK::I_TickedModuleCallEventContext &);

        void OnSysInit(SDK::I_TickedModuleCallEventContext &);
        void OnSysShutdown(SDK::I_TickedModuleCallEventContext &);

        void OnAppActivate(SDK::I_TickedModuleCallEventContext &);
        void OnAppDeactivate(SDK::I_TickedModuleCallEventContext &);

        static void StaticRegister(Module *);
        static void StaticHandleShutdown(Module *);
    };

    extern std::unique_ptr<Module> gModule;
} // namespace MafiaMP::Game