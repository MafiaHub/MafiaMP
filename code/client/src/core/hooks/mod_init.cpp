#include <utils/safe_win32.h>
#include <MinHook.h>
#include <utils/hooking/hook_function.h>

#include "../application_module.h"

#include "sdk/patterns.h"
#include "sdk/mafia/ui/c_game_gui_2_module.h"
#include "sdk/mafia/ui/menu/c_save_menu.h"

typedef void(__fastcall *C_InitDone__Init_MafiaFramework_t)(void *_this);
C_InitDone__Init_MafiaFramework_t C_InitDone__Init_MafiaFramework_original = nullptr;
void __fastcall C_InitDone__MafiaFramework(void *_this) {
    C_InitDone__Init_MafiaFramework_original(_this);
    MafiaMP::Core::gApplicationModule = new MafiaMP::Core::ApplicationModule();
}

int __fastcall C_CommandLine__FindCommand(void *_this, const char *command) {
    if (strstr(command, "NoMy2K") || strstr(command, "SkipLoadingPrompt") /*|| strstr(command, "fastRender")*/) {
        return 1;
    }
    return -1;
}

// This hooks should be disabled on release build - We should instead find a proper way to replace menu and bypass it
void __fastcall C_MainMenu__QueueMenuSequenceScreen(void *_this, int sequence, bool cannotSkip) {
    Framework::Logging::GetLogger("Hooks")->debug("[MainMenu::QueueMenuSequenceScreen]: sequence {}, cannotSkip {}", sequence, cannotSkip);
    if (sequence == -1) {
        auto menu     = SDK::mafia::ui::GetGameGui2Module();
        auto saveMenu = reinterpret_cast<SDK::mafia::ui::menu::C_MenuSave *>((uint64_t)menu->GetMainMenu());
        SDK::ue::C_String freeride("02_lost_heaven/lh_freeride_extreme.sav");
        saveMenu->OpenDebugLoadChapterString(freeride, false);
    }
}

typedef volatile signed __int32 *(__fastcall *C_MainMenu__RunGame_t)(void *pThis, const char *, const char *);
C_MainMenu__RunGame_t C_MainMenu__RunGame_original = nullptr;
volatile signed __int32 *__fastcall C_MainMenu__RunGame(void *pThis, const char *mission, const char *part) {
    Framework::Logging::GetLogger("Hooks")->debug("[MainMenu::RunGame]: mission {}, part {}", mission, part);
    return C_MainMenu__RunGame_original(pThis, mission, part);
}


static InitFunction init([]() {
    MH_CreateHook((LPVOID)SDK::gPatterns.C_InitDone_MafiaFramework, (PBYTE)C_InitDone__MafiaFramework, reinterpret_cast<void **>(&C_InitDone__Init_MafiaFramework_original));

    // Disable the loading intro
    hook::return_function(SDK::gPatterns.LoadIntro);

    // Bypass the main menu and auto load the freeride chapter
    const auto C_MainMenu__QueueMenuSequenceScreenAddr = hook::pattern("48 8B C4 48 89 58 ? 57 48 83 EC ? 48 8B D9 89 50 ?").get_first();
    MH_CreateHook((LPVOID)C_MainMenu__QueueMenuSequenceScreenAddr, (PBYTE)C_MainMenu__QueueMenuSequenceScreen, nullptr);

    // Listen for the game run calls
    const auto C_MainMenu__RunGame_Addr = hook::get_pattern("40 55 53 56 57 41 54 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 4C 8B 35");
    MH_CreateHook((LPVOID)C_MainMenu__RunGame_Addr, (PBYTE)C_MainMenu__RunGame, reinterpret_cast<void **>(&C_MainMenu__RunGame_original));

    // Skip loading prompt & debug stuff
    MH_CreateHook((LPVOID)SDK::gPatterns.C_CommandLine__FindCommand, (PBYTE)C_CommandLine__FindCommand, nullptr);
    },
    "ModInit");
