#pragma once

#include <utils/safe_win32.h>
#include <functional>
#include <Windows.h>
#include <dxgi.h>

#include "application.h"
#include "sdk/c_ticked_module.h"
#include "sdk/c_ticked_module_manager.h"
#include "sdk/ue/sys/render/device/c_direct_3d11_render_device.h"

namespace MafiaMP::Core {
    struct ApplicationModuleContext {
        HWND windowHandle                                                    = nullptr;
        IDXGISwapChain *swapChain                                            = nullptr;
        SDK::ue::sys::render::device::C_Direct3D11RenderDevice *renderDevice = nullptr;
    };

    class ApplicationModule: public SDK::C_TickedModule {
      public:
        ApplicationModule();

        void OnRender(SDK::I_TickedModuleCallEventContext &);
        void OnTick(SDK::I_TickedModuleCallEventContext &);

        void OnSysInit(SDK::I_TickedModuleCallEventContext &);
        void OnSysShutdown(SDK::I_TickedModuleCallEventContext &);

        static void StaticRegister(ApplicationModule *);
        static void StaticHandleShutdown(ApplicationModule *);
    };

    extern ApplicationModule *gApplicationModule;
    extern ApplicationModuleContext gApplicationModuleContext;
} // namespace MafiaMP::Game
