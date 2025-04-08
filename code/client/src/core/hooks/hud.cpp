#include <utils/safe_win32.h>
#include <MinHook.h>

#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include "sdk/mafia/ui/menu/c_menu_base.h"

typedef int64_t(__fastcall *C_MenuBase__OnScriptedMenuEvent_t)(void *pThis, int, SDK::mafia::ui::menu::C_MenuBase::E_ScriptedMenuEvent);
C_MenuBase__OnScriptedMenuEvent_t C_MenuBase__OnScriptedMenuEvent_original = nullptr;
int64_t C_MenuBase__OnScriptedMenuEvent(void *pThis, int unk, SDK::mafia::ui::menu::C_MenuBase::E_ScriptedMenuEvent menuEvent) {
    // TODO: broadcast over network
    Framework::Logging::GetLogger("Hooks")->debug("C_MenuBase::OnScriptedMenuEvent: unk {} event {}", unk, menuEvent);
    return C_MenuBase__OnScriptedMenuEvent_original(pThis, unk, menuEvent);
}

static InitFunction init([]() {
    const auto C_MenuBase__OnScriptedMenuEvent_Addr = hook::get_opcode_address("E8 ? ? ? ? 83 BE ? ? ? ? ? 75 0E");
    MH_CreateHook((LPVOID)C_MenuBase__OnScriptedMenuEvent_Addr, (PBYTE)C_MenuBase__OnScriptedMenuEvent, reinterpret_cast<void **>(&C_MenuBase__OnScriptedMenuEvent_original));
    },
    "HUD");
