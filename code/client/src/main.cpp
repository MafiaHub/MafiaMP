#include <Windows.h>

#include <MinHook.h>

#include <logging/logger.h>
#include <utils/hooking/hooking.h>
#include <utils/hooking/hook_function.h>
#include <utils/memory.h>

HINSTANCE gBaseInstance;

extern "C" void __declspec(dllexport) PreInit(HINSTANCE base) {
    // First, we flag the whole memory as read write
    Framework::Utils::MarkMemoryRW((uint8_t *)base);

    // Init the hooking library
    hook::set_base((uintptr_t)base);

    // Init minhook
    if (MH_Initialize() != MH_OK)
		return;
}

extern "C" void __declspec(dllexport) Init(HINSTANCE base) {
    Framework::Logging::GetLogger("Client")->info("Calling client Init function");

    // Run the init functions
    InitFunction::RunAll();
}

extern "C" void __declspec(dllexport) PostInit(HINSTANCE base) {
    Framework::Logging::GetLogger("Client")->info("Calling client PostInit function");
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID) {
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(instance);
        gBaseInstance = instance;
    } break;

    case DLL_PROCESS_DETACH: {
        MH_Uninitialize();
    }

    default: return FALSE;
    }

    return TRUE;
}