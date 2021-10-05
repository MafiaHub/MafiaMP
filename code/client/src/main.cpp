#include <Windows.h>

#include <logging/logger.h>
#include <MinHook.h>
#include <utils/hooking/hooking.h>
#include <utils/hooking/hooking_patterns.h>

int __fastcall C_CommandLine__FindCommand(void *_this, const char *command) {
    if (strstr(command, "NoMy2K") ||
        strstr(command, "SkipLoadingPrompt") || 
        strstr(command, "fastRender") ) {
        return 1;
    }
    return -1;
}

extern "C" void __declspec(dllexport) InitClient() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

     const auto LoadIntroAddr = hook::pattern("48 89 5C 24 ? 55 56 57 48 83 EC ? 48 8B E9 B9 ? ? ? ?").get_first();
    hook::return_function(LoadIntroAddr);

    //NOTE: skip loading prompt & debug stuff
    const auto C_CommandLine__FindCommandAddr = hook::get_opcode_address("E8 ? ? ? ? 40 88 7D BB");
    MH_CreateHook((LPVOID)C_CommandLine__FindCommandAddr, (PBYTE)C_CommandLine__FindCommand, nullptr);
}
