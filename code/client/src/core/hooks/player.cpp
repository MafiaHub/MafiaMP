#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include "sdk/entities/c_player_2.h"
#include "sdk/c_game.h"

typedef bool(__fastcall *C_Player2__IsInputDisabled_t)(SDK::C_Player2 *, int);
C_Player2__IsInputDisabled_t C_Player2__IsInputDisabled_Original = nullptr;
bool __fastcall C_Player2__IsInputDisabled(SDK::C_Player2 *pPlayer2, int DisabledInput) {
    return C_Player2__IsInputDisabled_Original(SDK::GetGame()->GetActivePlayer(), DisabledInput);
}

typedef void *(__fastcall *C_Player2__GetStamina_t)(SDK::C_Player2 *);
C_Player2__GetStamina_t C_Player2__GetStamina_Original = nullptr;
void *__fastcall C_Player2__GetStamina(SDK::C_Player2 *pPlayer2) {
    return C_Player2__GetStamina_Original(SDK::GetGame()->GetActivePlayer());
}

static InitFunction init([]() {
    // These hooks are required to ensure the game calls these methods only on the local player pointer and not on any other entity
    // Used for character controller overrides - DO NOT DELETE
    const auto C_Player2__IsInputDisabledAddr = hook::pattern("4C 63 C2 49 8D 40 66").get_first();
    MH_CreateHook((LPVOID)C_Player2__IsInputDisabledAddr, (PBYTE)C_Player2__IsInputDisabled, reinterpret_cast<void **>(&C_Player2__IsInputDisabled_Original));

    const auto C_Player2__GetStaminaAddr = hook::get_opcode_address("E8 ? ? ? ? 48 8B C8 44 8D 43 01");
    MH_CreateHook((LPVOID)C_Player2__GetStaminaAddr, (PBYTE)C_Player2__GetStamina, reinterpret_cast<void **>(&C_Player2__GetStamina_Original));
    },
    "Player");
