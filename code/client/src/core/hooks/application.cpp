#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

bool __fastcall C_GameFramework__IsSuspended(void *_this) {
    return false;
}

static InitFunction init([]() {
    // Don't pause the game when out of window
    const auto C_GameFramework__IsSuspendedAddr = hook::pattern("80 39 ? 74 ? 80 79 ? ?").get_first();
    MH_CreateHook((LPVOID)C_GameFramework__IsSuspendedAddr, (PBYTE)&C_GameFramework__IsSuspended, nullptr);
});
