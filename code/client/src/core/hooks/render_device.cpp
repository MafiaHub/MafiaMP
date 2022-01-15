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
#include "../../sdk/ue/sys/render/device/c_d3d11_window_context_cache.h"
#include "../../sdk/ue/c_application_win32.h"


typedef bool(__fastcall *C_Direct3D11RenderDevice__Init_t)(SDK::ue::sys::render::device::C_Direct3D11RenderDevice *pThis, SDK::ue::sys::render::device::S_RenderDeviceDesc const &a2, SDK::ue::sys::render::device::C_DynamicVIBufferPool &a3, void *idk);
C_Direct3D11RenderDevice__Init_t C_Direct3D11RenderDevice__Init_original = nullptr;

typedef int64_t(__fastcall *C_WindowProcHandler__CreateMainWindow_t)(void *_this, SDK::ue::C_Application_Win32 *appWin32);
C_WindowProcHandler__CreateMainWindow_t C_WindowProcHandler__CreateMainWindow_original = nullptr;

typedef bool(__fastcall *C_D3D11WindowContextCache__InitSwapChainInternal_t)(void *_this, SDK::ue::sys::render::device::C_D3D11WindowContextCache::S_WndContextDesc &);
C_D3D11WindowContextCache__InitSwapChainInternal_t C_D3D11WindowContextCache__InitSwapChainInternal_original = nullptr;

static HRESULT (*D3D11Present_original)(IDXGISwapChain *, UINT, UINT) = nullptr;

HRESULT D3D11Present_Hook(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags) {
    if (!(flags & DXGI_PRESENT_TEST)) {
        const auto app = MafiaMP::Core::gApplication.get();
        if (app && app->IsInitialized() && MafiaMP::Game::gTargetView) {
            MafiaMP::Game::gRenderDevice->_context->OMSetRenderTargets(1, &MafiaMP::Game::gTargetView, NULL);
            app->GetImGUI()->Render();
        }
    }

    return D3D11Present_original(swapChain, syncInterval, flags);
}


bool C_Direct3D11RenderDevice__Init(SDK::ue::sys::render::device::C_Direct3D11RenderDevice *device, SDK::ue::sys::render::device::S_RenderDeviceDesc const &a2,
    SDK::ue::sys::render::device::C_DynamicVIBufferPool &a3, void *idk) {
    auto result = C_Direct3D11RenderDevice__Init_original(device, a2, a3, idk);

    // Store the device for later init, since it's the first thing to initialize in the game
    MafiaMP::Game::gRenderDevice = device;
    return result;
}

int64_t C_WindowProcHandler__CreateMainWindow(void* _this, SDK::ue::C_Application_Win32* appWin32) {
    auto result = C_WindowProcHandler__CreateMainWindow_original(_this, appWin32);

    // Store the window pointer for later init
    MafiaMP::Game::gWindow = appWin32->m_pWindow;
    return result;
}

bool C_D3D11WindowContextCache__InitSwapChainInternal(void *_this, SDK::ue::sys::render::device::C_D3D11WindowContextCache::S_WndContextDesc &desc) {
    auto result = C_D3D11WindowContextCache__InitSwapChainInternal_original(_this, desc);

    // Store the swapchain pointer for later init
    MafiaMP::Game::gSwapChain = desc.m_pSwapChain;

    // Patch the swap chain present method to render our draw data (Enable hook is mandatory since it's happening dynamically, after static hooks initialize)
    auto pSwapChainVtable     = (DWORD_PTR *)MafiaMP::Game::gSwapChain;
    pSwapChainVtable          = (DWORD_PTR *)pSwapChainVtable[0];
    auto pDeviceContextVTable = (DWORD_PTR *)MafiaMP::Game::gRenderDevice->_context;
    pDeviceContextVTable      = (DWORD_PTR *)pDeviceContextVTable[0];
    MH_CreateHook((LPVOID)pSwapChainVtable[8], (PBYTE)D3D11Present_Hook, reinterpret_cast<void **>(&D3D11Present_original));
    MH_EnableHook(MH_ALL_HOOKS);
    return result;
}

static InitFunction init([]() {
    const auto C_Direct3D11RenderDevice__Init_Addr = reinterpret_cast<uint64_t>(hook::pattern("4C 89 44 24 ? 55 53 56 57 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 10 02").get_first());
    MH_CreateHook((LPVOID)C_Direct3D11RenderDevice__Init_Addr, (PBYTE)C_Direct3D11RenderDevice__Init, reinterpret_cast<void **>(&C_Direct3D11RenderDevice__Init_original));

    const auto C_WindowProcHandler__CreateMainWindow__Addr = hook::get_opcode_address("E8 ? ? ? ? 48 8B C7 48 8B B4 24 ? ? ? ? 48 83 C4 60");
    MH_CreateHook((LPVOID)C_WindowProcHandler__CreateMainWindow__Addr, (PBYTE)C_WindowProcHandler__CreateMainWindow, reinterpret_cast<void **>(&C_WindowProcHandler__CreateMainWindow_original));

    const auto initSwapChainPattern = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 48 89 74 24 ? 55 48 8B EC 48 83 EC 70 48 8B F1").get_first());
    MH_CreateHook((LPVOID)initSwapChainPattern, (PBYTE)C_D3D11WindowContextCache__InitSwapChainInternal, reinterpret_cast<void **>(&C_D3D11WindowContextCache__InitSwapChainInternal_original));
});
