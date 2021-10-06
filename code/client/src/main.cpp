#include <MinHook.h>
#include <Windows.h>
#include <logging/logger.h>
#include <utils/hooking/hook_function.h>

extern "C" void __declspec(dllexport) InitClient() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    InitFunction::RunAll();
}
