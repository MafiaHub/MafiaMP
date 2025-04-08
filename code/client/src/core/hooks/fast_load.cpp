#include <MinHook.h>
#include <utils/hooking/hooking.h>
#include <utils/hooking/hook_function.h>

#include "sdk/mafia/ui/c_game_gui_2_module.h"
#include "sdk/mafia/ui/menu/c_save_menu.h"

void __fastcall C_MainMenu__QueueMenuSequenceScreen(void *_this, int sequence, bool cannotSkip) {
    if (sequence == -1) {
        auto menu     = SDK::mafia::ui::GetGameGui2Module();
        auto saveMenu = reinterpret_cast<SDK::mafia::ui::menu::C_MenuSave *>((uint64_t)menu->GetMainMenu());
        SDK::ue::C_String freeride("02_lost_heaven/lh_freeride_extreme.sav");
        saveMenu->OpenDebugLoadChapterString(freeride, false);
    }
}

static InitFunction init([]() {
    // This hooks should be disabled on release build - We should instead find a proper way to replace menu and bypass it
    const auto C_MainMenu__QueueMenuSequenceScreenAddr = hook::pattern("48 8B C4 48 89 58 ? 57 48 83 EC ? 48 8B D9 89 50 ?").get_first();
    MH_CreateHook((LPVOID)C_MainMenu__QueueMenuSequenceScreenAddr, (PBYTE)C_MainMenu__QueueMenuSequenceScreen, nullptr);
    },
    "FastLoad");
