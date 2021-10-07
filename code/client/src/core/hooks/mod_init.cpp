#include "../../game/module.h"
#include "../../sdk/patterns.h"

#include <MinHook.h>
#include <utils/hooking/hooking.h>
#include <utils/hooking/hook_function.h>

// Ticked module hooking
typedef void(__fastcall *C_InitDone__Init_MafiaFramework_t)(void *_this);
C_InitDone__Init_MafiaFramework_t C_InitDone__Init_MafiaFramework_original = nullptr;
void __fastcall C_InitDone__MafiaFramework(void *_this) {
    C_InitDone__Init_MafiaFramework_original(_this);
    MafiaMP::Game::gModule = new MafiaMP::Game::Module();
}

// Intro vide hooking
int __fastcall C_CommandLine__FindCommand(void *_this, const char *command) {
    if (strstr(command, "NoMy2K") || strstr(command, "SkipLoadingPrompt") /*|| strstr(command, "fastRender")*/) {
        return 1;
    }
    return -1;
}

static InitFunction init([]() {
    MH_CreateHook((LPVOID)SDK::gPatterns.C_InitDone_MafiaFrameworkAddr, (PBYTE)C_InitDone__MafiaFramework, reinterpret_cast<void **>(&C_InitDone__Init_MafiaFramework_original));

    // Disable the loading intro
    hook::return_function(SDK::gPatterns.LoadIntroAddr);

    // Skip loading prompt & debug stuff
    MH_CreateHook((LPVOID)SDK::gPatterns.C_CommandLine__FindCommandAddr, (PBYTE)C_CommandLine__FindCommand, nullptr);
});
