#include "../application_module.h"
#include "sdk/patterns.h"

#include <MinHook.h>
#include <utils/hooking/hook_function.h>

// Ticked module hooking
typedef void(__fastcall *C_InitDone__Init_MafiaFramework_t)(void *_this);
C_InitDone__Init_MafiaFramework_t C_InitDone__Init_MafiaFramework_original = nullptr;
void __fastcall C_InitDone__MafiaFramework(void *_this) {
    C_InitDone__Init_MafiaFramework_original(_this);
    MafiaMP::Core::gApplicationModule = new MafiaMP::Core::ApplicationModule();
}

// Intro video hooking
int __fastcall C_CommandLine__FindCommand(void *_this, const char *command) {
    if (strstr(command, "NoMy2K") || strstr(command, "SkipLoadingPrompt") /*|| strstr(command, "fastRender")*/) {
        return 1;
    }
    return -1;
}

static InitFunction init([]() {
    MH_CreateHook((LPVOID)SDK::gPatterns.C_InitDone_MafiaFramework, (PBYTE)C_InitDone__MafiaFramework, reinterpret_cast<void **>(&C_InitDone__Init_MafiaFramework_original));

    // Disable the loading intro
    hook::return_function(SDK::gPatterns.LoadIntro);

    // Skip loading prompt & debug stuff
    MH_CreateHook((LPVOID)SDK::gPatterns.C_CommandLine__FindCommand, (PBYTE)C_CommandLine__FindCommand, nullptr);
    },
    "ModInit");
