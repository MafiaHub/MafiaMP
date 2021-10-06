#include "../module.h"

#include <MinHook.h>
#include <utils/hooking/hooking.h>
#include <utils/hooking/hook_function.h>

// Ticked module hooking
typedef void(__fastcall *C_InitDone__Init_MafiaFramework_t)(void *_this);
C_InitDone__Init_MafiaFramework_t C_InitDone__Init_MafiaFramework_original = nullptr;
void __fastcall C_InitDone__MafiaFramework(void *_this) {
    C_InitDone__Init_MafiaFramework_original(_this);
    MafiaMP::Game::gModule.reset(new MafiaMP::Game::Module());
}

// Intro vide hooking
int __fastcall C_CommandLine__FindCommand(void *_this, const char *command) {
    if (strstr(command, "NoMy2K") || strstr(command, "SkipLoadingPrompt") || strstr(command, "fastRender")) {
        return 1;
    }
    return -1;
}

static InitFunction init([]() {
    // TODO: move pattern to patterns file
    const auto C_InitDone_MafiaFrameworkAddr = hook::pattern("40 55 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 57 C0").get_first();
    MH_CreateHook((LPVOID)C_InitDone_MafiaFrameworkAddr, (PBYTE)C_InitDone__MafiaFramework, reinterpret_cast<void **>(&C_InitDone__Init_MafiaFramework_original));

    // Disable the loading intro
    const auto LoadIntroAddr = hook::pattern("48 89 5C 24 ? 55 56 57 48 83 EC ? 48 8B E9 B9 ? ? ? ?").get_first();
    hook::return_function(LoadIntroAddr);

    // Skip loading prompt & debug stuff
    const auto C_CommandLine__FindCommandAddr = hook::get_opcode_address("E8 ? ? ? ? 40 88 7D BB");
    MH_CreateHook((LPVOID)C_CommandLine__FindCommandAddr, (PBYTE)C_CommandLine__FindCommand, nullptr);
});
