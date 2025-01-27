#include <MinHook.h>
#include <utils/hooking/hook_function.h>

#include "sdk/patterns.h"
#include "sdk/ue/c_application_win32.h"

bool __fastcall C_GameFramework__IsSuspended(void *_this) {
    return false;
}

typedef int64_t(__fastcall *C_Application_Win32__Initialize_t)(SDK::ue::C_Application_Win32 *pThis);
C_Application_Win32__Initialize_t C_Application_Win32__Initialize_original = nullptr;

int64_t __fastcall C_Application_Win32__Initialize(SDK::ue::C_Application_Win32 *pThis) {
    const auto result = C_Application_Win32__Initialize_original(pThis);
    Framework::Logging::GetLogger("Hooks")->info("Application Initialized (FPS {}/{}) (Unique ID {})", pThis->m_nFPSMin, pThis->m_nFPSMax, pThis->m_sUniqueId);
    Framework::Logging::GetLogger("Hooks")->debug("Application command line size: {}", pThis->m_pCommandLine->m_vCommands.size());

    return result;
}

typedef int64_t(__fastcall *C_Application__Run_t)(SDK::ue::C_Application *pThis);
C_Application__Run_t C_Application__Run_original = nullptr;

int64_t __fastcall C_Application__Run(SDK::ue::C_Application *pThis) {
    return C_Application__Run_original(pThis);
}

static InitFunction init([]() {
    // Don't pause the game when out of window
    MH_CreateHook((LPVOID)SDK::gPatterns.C_GameFramework__IsSuspended, (PBYTE)&C_GameFramework__IsSuspended, nullptr);

    // Hook the application initialize method
        const auto C_Application_Win32__Initialize_Addr = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? 48 8B D9 FF 15 ? ? ? ? 48 8B 4B"));
        MH_CreateHook((LPVOID)C_Application_Win32__Initialize_Addr, (PBYTE)C_Application_Win32__Initialize, reinterpret_cast<void **>(&C_Application_Win32__Initialize_original));

        const auto C_Application__Run_Addr = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B 01 48 8B D9 FF 90"));
        MH_CreateHook((LPVOID)C_Application__Run_Addr, (PBYTE)C_Application__Run, reinterpret_cast<void **>(&C_Application__Run_original));
    },
    "Application");
