#pragma once

#include <utils/safe_win32.h>

#include "../core/application.h"

#include "../sdk/c_ticked_module.h"
#include "../sdk/c_ticked_module_manager.h"

#include "../sdk/ue/sys/render/device/c_direct_3d11_render_device.h"

#include <functional>
#include <Windows.h>

#include <dxgi.h>

namespace MafiaMP::Game {
    class ModModule: public SDK::C_TickedModule {
      public:
        ModModule();

        void OnGameRender(SDK::I_TickedModuleCallEventContext &);
        void OnGameTick(SDK::I_TickedModuleCallEventContext &);

        void OnSysInit(SDK::I_TickedModuleCallEventContext &);
        void OnSysShutdown(SDK::I_TickedModuleCallEventContext &);

        static void StaticRegister(ModModule *);
        static void StaticHandleShutdown(ModModule *);
    };

    struct Globals {
        ModModule *module                                                    = nullptr;
        HWND window                                                          = nullptr;
        IDXGISwapChain *swapChain                                            = nullptr;
        SDK::ue::sys::render::device::C_Direct3D11RenderDevice *renderDevice = nullptr;
    };

    extern Globals gGlobals;
} // namespace MafiaMP::Game
