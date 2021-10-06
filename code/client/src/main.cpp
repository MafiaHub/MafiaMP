#include <MinHook.h>
#include <Windows.h>
#include <logging/logger.h>

#include <utils/hooking/hooking_patterns.h>
#include <utils/hooking/hook_function.h>

#include "sdk/patterns.h"

extern "C" void __declspec(dllexport) InitClient() {
    // Initialize and allocate the console
    AllocConsole();
	AttachConsole(GetCurrentProcessId());
	SetConsoleTitleW(L"MafiaMP - Dev Console");

	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

    MH_Initialize();
	hook::set_base();
    hook::load_hints();

    // Load the patterns
    SDK::Patterns::InitPatterns();

    // Entry point is handled by an InitFunction, so we just have to enable hooks and trigger the shits down here
    InitFunction::RunAll();
    MH_EnableHook(MH_ALL_HOOKS);
}
