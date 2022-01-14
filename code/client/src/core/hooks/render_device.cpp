#include <MinHook.h>
#include <utils/hooking/hooking.h>
#include <utils/hooking/hook_function.h>

#include <logging/logger.h>

#include <d3d11.h>

#include "../application.h"
#include "../../game/module.h"

#include "../../sdk/ue/sys/render/device/c_direct_3d11_render_device.h"
#include "../../sdk/ue/sys/render/device/s_render_device_desc.h"
#include "../../sdk/ue/sys/render/device/c_dynamic_vi_buffer_pool.h"
#include "../../sdk/ue/c_application_win32.h"


typedef bool(__fastcall *C_RenderDevice_Unknown_t)(SDK::ue::sys::render::device::C_Direct3D11RenderDevice *pThis, SDK::ue::sys::render::device::S_RenderDeviceDesc const &a2, SDK::ue::sys::render::device::C_DynamicVIBufferPool &a3, void *idk);
C_RenderDevice_Unknown_t C_RenderDevice_Unknown_original = nullptr;

typedef int64_t(__fastcall *C_WindowProcHandler__CreateMainWindow_t)(void *_this, SDK::ue::C_Application_Win32 *appWin32);
C_WindowProcHandler__CreateMainWindow_t C_WindowProcHandler__CreateMainWindow_original = nullptr;

bool C_RenderDevice_Unknown(SDK::ue::sys::render::device::C_Direct3D11RenderDevice *device, SDK::ue::sys::render::device::S_RenderDeviceDesc const &a2,
    SDK::ue::sys::render::device::C_DynamicVIBufferPool &a3, void *idk) {

    // Store the device for later init, since it's the first thing to initialize in the game
    MafiaMP::Game::gRenderDevice = device;
    return C_RenderDevice_Unknown_original(device, a2, a3, idk);
}

int64_t C_WindowProcHandler__CreateMainWindow(void* _this, SDK::ue::C_Application_Win32* appWin32) {
    auto result = C_WindowProcHandler__CreateMainWindow_original(_this, appWin32);

    // Store the window pointer for later init
    MafiaMP::Game::gWindow = appWin32->m_pWindow;
    return result;
}

static InitFunction init([]() {
    const auto C_Unknown_Addr = hook::get_opcode_address("E8 ? ? ? ? 0F 10 03 33 ED");
    MH_CreateHook((LPVOID)C_Unknown_Addr, (PBYTE)C_RenderDevice_Unknown, reinterpret_cast<void **>(&C_RenderDevice_Unknown_original));

    const auto C_WindowProcHandler__CreateMainWindow__Addr = hook::get_opcode_address("E8 ? ? ? ? 48 8B C7 48 8B B4 24 ? ? ? ? 48 83 C4 60");
    MH_CreateHook((LPVOID)C_WindowProcHandler__CreateMainWindow__Addr, (PBYTE)C_WindowProcHandler__CreateMainWindow,
        reinterpret_cast<void **>(&C_WindowProcHandler__CreateMainWindow_original));
});
