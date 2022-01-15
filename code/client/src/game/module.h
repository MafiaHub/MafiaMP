#pragma once

#include "../sdk/c_ticked_module.h"
#include "../sdk/c_ticked_module_manager.h"

#include "../sdk/ue/sys/render/device/c_direct_3d11_render_device.h"

#include <functional>
#include <Windows.h>

namespace MafiaMP::Game {
    class Module: public SDK::C_TickedModule {
      public:
        Module();

        void OnGameRender(SDK::I_TickedModuleCallEventContext &);
        void OnGameTick(SDK::I_TickedModuleCallEventContext &);

        void OnSysInit(SDK::I_TickedModuleCallEventContext &);
        void OnSysShutdown(SDK::I_TickedModuleCallEventContext &);

        static void StaticRegister(Module *);
        static void StaticHandleShutdown(Module *);
    };

    extern Module* gModule;
    extern SDK::ue::sys::render::device::C_Direct3D11RenderDevice *gRenderDevice;
    extern HWND gWindow;
} // namespace MafiaMP::Game
