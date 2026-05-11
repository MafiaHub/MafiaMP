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
#include "sdk/ue/sys/render/device/s_device_adapter_desc.h"
#include "sdk/ue/sys/render/device/s_render_device_desc.h"
#include "sdk/ue/sys/render/device/s_video_mode_desc.h"

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

static const char *VendorClassName(uint32_t vendorClass) {
    switch (vendorClass) {
    case 0: return "NVIDIA";
    case 1: return "AMD";
    case 2: return "Other";
    default: return "?";
    }
}

bool C_Direct3D11RenderDevice__Init(SDK::ue::sys::render::device::C_Direct3D11RenderDevice *device, SDK::ue::sys::render::device::S_RenderDeviceDesc const &desc, SDK::ue::sys::render::device::C_DynamicVIBufferPool &pool, void *outCtx) {
    auto log = Framework::Logging::GetLogger("Hooks");

    // Bit 17 (m_bFullscreen) is decoded; the other four still need names.
    const uint32_t unknownFlagBits = (desc.m_bFlagBit01 ? 0x01 : 0)
                                  | (desc.m_bFlagBit12 ? 0x02 : 0)
                                  | (desc.m_bFlagBit15 ? 0x04 : 0)
                                  | (desc.m_bFlagBit16 ? 0x08 : 0);

    log->debug("[D3D11RenderDevice::Init] requested: backbuffer={}x{} adapter={} mode={} fullscreen={} swap-effect={} unknown-flags=0x{:02X}",
               desc.m_nBackBufferWidth, desc.m_nBackBufferHeight,
               desc.m_nAdapterIdx, desc.m_nVideoModeIdx,
               desc.m_bFullscreen,
               static_cast<uint32_t>(desc.m_eSwapEffect), unknownFlagBits);

    auto result = C_Direct3D11RenderDevice__Init_original(device, desc, pool, outCtx);

    log->debug("[D3D11RenderDevice::Init] result={}", result);
    if (result && device) {
        const uint32_t adapterCount = device->GetAdapterCount();
        log->debug("[D3D11RenderDevice::Init] hardware={} force-warp={} feature-level=0x{:X} adapters={}",
                   device->IsHardwareDevice(), device->m_bForceWARP,
                   static_cast<uint32_t>(device->m_eFeatureLevel), adapterCount);
        log->debug("[D3D11RenderDevice::Init] actual backbuffer={}x{} current-adapter={} current-mode={}",
                   device->GetCurrentBackBufferWidth(), device->GetCurrentBackBufferHeight(),
                   device->m_nCurrentAdapterIdx, device->m_nCurrentVideoModeIdx);

        for (uint32_t i = 0; i < adapterCount; ++i) {
            const auto *adapter = device->GetAdapter(i);
            if (!adapter) continue;
            const uint32_t modeCount = adapter->GetNumModes();
            log->debug("[D3D11RenderDevice::Init]   adapter[{}] vendor=0x{:04X} device=0x{:04X} subsys=0x{:08X} rev=0x{:X} class={} modes={}",
                       i, adapter->m_nVendorId, adapter->m_nDeviceId,
                       adapter->m_nSubSysId, adapter->m_nRevision,
                       VendorClassName(adapter->m_eVendorClass), modeCount);
            if (adapter->m_pName) {
                log->debug("[D3D11RenderDevice::Init]     fake-name='{}'", adapter->m_pName);
            }
            // Show first / last / current modes so the log stays compact even on adapters with hundreds of modes.
            auto logMode = [&](const char *tag, uint32_t idx) {
                if (idx >= modeCount) return;
                const auto *m = adapter->GetMode(idx);
                log->debug("[D3D11RenderDevice::Init]     {}[{}] {}x{} @ {}Hz aspect={:.3f} fmt={}",
                           tag, idx, m->m_nWidth, m->m_nHeight, m->m_nFrequency,
                           m->m_fAspectRatio, static_cast<uint32_t>(m->m_eFormat));
            };
            if (modeCount > 0) {
                logMode("first", 0);
                if (modeCount > 1) logMode("last", modeCount - 1);
                if (i == device->m_nCurrentAdapterIdx) logMode("current", device->m_nCurrentVideoModeIdx);
            }
        }
    }

    MafiaMP::Core::gApplicationModuleContext.renderDevice = device;

    return result;
}

int64_t C_WindowProcHandler__CreateMainWindow(void *_this, SDK::ue::C_Application_Win32 *appWin32) {
    auto result = C_WindowProcHandler__CreateMainWindow_original(_this, appWin32);
    auto log    = Framework::Logging::GetLogger("Hooks");

    const auto &setup = appWin32->m_windowSetup;
    log->debug("[CreateMainWindow] hwnd=0x{:X} {}x{} pos=({},{}) display-mode={} vsync={} aa={} initialized={}",
               reinterpret_cast<uintptr_t>(appWin32->m_hWindow),
               setup.m_nWidth, setup.m_nHeight,
               setup.m_nWindowX, setup.m_nWindowY,
               setup.m_nDisplayMode, setup.m_nVSync,
               setup.m_nAntiAliasing, setup.m_bInitialized);
    // Dump the four "Lo/Hi" pair fields as raw hex: their existing names (RefreshRate/MonitorIndex)
    // don't match observed data (the high half of one of them carries the video-mode index in 0x3E format),
    // so we keep them visible but un-interpreted until they're reverse-engineered properly.
    log->debug("[CreateMainWindow] raw pair fields: 0x{:08X}/0x{:08X} 0x{:08X}/0x{:08X}",
               static_cast<uint32_t>(setup.m_nRefreshRateLo), static_cast<uint32_t>(setup.m_nRefreshRateHi),
               static_cast<uint32_t>(setup.m_nMonitorIndexLo), static_cast<uint32_t>(setup.m_nMonitorIndexHi));

    MafiaMP::Core::gApplicationModuleContext.windowHandle = appWin32->m_hWindow;

    SetWindowTextA(appWin32->m_hWindow, "Mafia: Multiplayer");
    g_pOriginalWndProcHandler = (WNDPROC)SetWindowLongPtrW(appWin32->m_hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

    return result;
}

bool C_D3D11WindowContextCache__InitSwapChainInternal(SDK::ue::sys::render::device::C_D3D11WindowContextCache *pThis, SDK::ue::sys::render::device::C_D3D11WindowContextCache::S_WndContextDesc &desc) {
    auto log = Framework::Logging::GetLogger("Hooks");

    log->debug("[D3D11WindowContext::InitSwapChain] hwnd=0x{:X} {}x{} fullscreen={} default-backbuffer={}",
               reinterpret_cast<uintptr_t>(desc.m_hWindow),
               desc.m_nWidth, desc.m_nHeight,
               desc.m_bFullscreen, desc.m_bCreateDefaultBackBuffer);

    auto result = C_D3D11WindowContextCache__InitSwapChainInternal_original(pThis, desc);

    log->debug("[D3D11WindowContext::InitSwapChain] result={} swap-chain=0x{:X}",
               result, reinterpret_cast<uintptr_t>(desc.m_pSwapChain));

    if (result && desc.m_pSwapChain) {
        DXGI_SWAP_CHAIN_DESC sc{};
        if (SUCCEEDED(desc.m_pSwapChain->GetDesc(&sc))) {
            log->debug("[D3D11WindowContext::InitSwapChain] swap-chain: {}x{} fmt={} refresh={}/{} buffers={} windowed={} swap-effect={}",
                       sc.BufferDesc.Width, sc.BufferDesc.Height,
                       static_cast<uint32_t>(sc.BufferDesc.Format),
                       sc.BufferDesc.RefreshRate.Numerator, sc.BufferDesc.RefreshRate.Denominator,
                       sc.BufferCount, sc.Windowed != 0,
                       static_cast<uint32_t>(sc.SwapEffect));
        }
    }

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
