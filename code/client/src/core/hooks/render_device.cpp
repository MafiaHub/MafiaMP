#include <utils/safe_win32.h>

#include "../application.h"
#include "../application_module.h"
#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include "game/helpers/game_input.h"

#include <d3d11.h>


#include "sdk/ue/c_application_win32.h"
#include "sdk/ue/sys/render/device/c_d3d11_window_context_cache.h"
#include "sdk/ue/sys/render/device/c_direct_3d11_render_device.h"
#include "sdk/ue/sys/render/device/c_dynamic_vi_buffer_pool.h"
#include "sdk/ue/sys/render/device/s_render_device_desc.h"

typedef bool(__fastcall *C_Direct3D11RenderDevice__Init_t)(SDK::ue::sys::render::device::C_Direct3D11RenderDevice *, SDK::ue::sys::render::device::S_RenderDeviceDesc const &, SDK::ue::sys::render::device::C_DynamicVIBufferPool &, void *);
C_Direct3D11RenderDevice__Init_t C_Direct3D11RenderDevice__Init_original = nullptr;

typedef int64_t(__fastcall *C_WindowProcHandler__CreateMainWindow_t)(void *, SDK::ue::C_Application_Win32 *);
C_WindowProcHandler__CreateMainWindow_t C_WindowProcHandler__CreateMainWindow_original = nullptr;

typedef bool(__fastcall *C_D3D11WindowContextCache__InitSwapChainInternal_t)(SDK::ue::sys::render::device::C_D3D11WindowContextCache *, SDK::ue::sys::render::device::C_D3D11WindowContextCache::S_WndContextDesc &);
C_D3D11WindowContextCache__InitSwapChainInternal_t C_D3D11WindowContextCache__InitSwapChainInternal_original = nullptr;

typedef HRESULT(__fastcall *D3D11Present_original)(IDXGISwapChain *, UINT, UINT);
D3D11Present_original g_pOriginalD3D11Present = nullptr;
WNDPROC g_pOriginalWndProcHandler             = nullptr;

HRESULT D3D11Present_Hook(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags) {
    if (!(flags & DXGI_PRESENT_TEST)) {
        const auto app = MafiaMP::Core::gApplication.get();
        if (app && app->IsInitialized()) {
            app->GetWebManager()->Render();
            app->GetRenderer()->Render();
            app->GetRenderer()->Paint();

            // ImGUI comes last, since it does not rely on our graphics renderer yet
            app->GetImGUI()->Render();
        }
    }

    return g_pOriginalD3D11Present(swapChain, syncInterval, flags);
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    const auto app = MafiaMP::Core::gApplication.get();
    if (app && app->IsInitialized()) {
        // Tick the input system from framework
        app->GetInput()->ProcessEvent(hWnd, msg, wParam, lParam);
        app->GetImGUI()->ProcessEvent(hWnd, msg, wParam, lParam);
    }

    switch (msg) {
    // Web handler inputs
    case WM_MOUSEWHEEL:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP: {
        if (app && app->IsInitialized()) {
            if (app->GetWebManager()) {
                app->GetWebManager()->ProcessMouseEvent(hWnd, msg, wParam, lParam);
            }
        }
    } break;

    case WM_CHAR:
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP: {
        if (app && app->IsInitialized()) {
            if (app->GetWebManager()) {
                app->GetWebManager()->ProcessKeyboardEvent(hWnd, msg, wParam, lParam);
            }
        }
    } break;

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY: ::PostQuitMessage(0); return 0;
    }

    return g_pOriginalWndProcHandler(hWnd, msg, wParam, lParam);
}

bool C_Direct3D11RenderDevice__Init(SDK::ue::sys::render::device::C_Direct3D11RenderDevice *device, SDK::ue::sys::render::device::S_RenderDeviceDesc const &a2, SDK::ue::sys::render::device::C_DynamicVIBufferPool &a3, void *idk) {
    auto result = C_Direct3D11RenderDevice__Init_original(device, a2, a3, idk);
    Framework::Logging::GetLogger("Hooks")->debug("D3D11 Render Device Init = {}", result);

    MafiaMP::Core::gApplicationModuleContext.renderDevice = device;

    return result;
}

int64_t C_WindowProcHandler__CreateMainWindow(void *_this, SDK::ue::C_Application_Win32 *appWin32) {
    auto result = C_WindowProcHandler__CreateMainWindow_original(_this, appWin32);
    Framework::Logging::GetLogger("Hooks")->debug("MainWindow Created");

    MafiaMP::Core::gApplicationModuleContext.windowHandle = appWin32->m_hWindow;

    SetWindowTextA(appWin32->m_hWindow, "Mafia: Multiplayer");
    g_pOriginalWndProcHandler = (WNDPROC)SetWindowLongPtrW(appWin32->m_hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

    return result;
}

bool C_D3D11WindowContextCache__InitSwapChainInternal(SDK::ue::sys::render::device::C_D3D11WindowContextCache *pThis, SDK::ue::sys::render::device::C_D3D11WindowContextCache::S_WndContextDesc &desc) {
    auto result = C_D3D11WindowContextCache__InitSwapChainInternal_original(pThis, desc);

    if (result) {
        MafiaMP::Core::gApplicationModuleContext.swapChain = desc.m_pSwapChain;
    }

    // Patch the swap chain present method to render our draw data (Enable hook is mandatory since it's happening dynamically, after static hooks initialize)
    auto pSwapChainVtable = (DWORD_PTR *)desc.m_pSwapChain;
    pSwapChainVtable      = (DWORD_PTR *)pSwapChainVtable[0];

    // TODO get rid of hook
    // g_pOriginalD3D11Present   = reinterpret_cast<D3D11Present_original>(pSwapChainVtable[8]);
    MH_CreateHook((LPVOID)pSwapChainVtable[8], (PBYTE)D3D11Present_Hook, reinterpret_cast<void **>(&g_pOriginalD3D11Present));
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
    },
    "RenderDevice");
