#include <Windows.h>

#include <logging/logger.h>

extern "C" void __declspec(dllexport) InitClient() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
}
