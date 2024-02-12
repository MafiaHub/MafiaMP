#include <MinHook.h>
#include <utils/hooking/hook_function.h>

#include "sdk/patterns.h"

bool __fastcall C_GameFramework__IsSuspended(void *_this) {
    return false;
}

static InitFunction init([]() {
    // Don't pause the game when out of window
    MH_CreateHook((LPVOID)SDK::gPatterns.C_GameFramework__IsSuspended, (PBYTE)&C_GameFramework__IsSuspended, nullptr);
});
