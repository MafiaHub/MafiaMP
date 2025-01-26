#include <utils/safe_win32.h>

#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include "sdk/c_player_teleport_module.h"
#include "sdk/entities/c_actor.h"
#include "sdk/entities/c_car.h"
#include "sdk/entities/c_player_2.h"
#include "sdk/ue/game/humainai/c_character_controller.h"

#include "game/helpers/controls.h"

#include "core/application.h"
#include "shared/game_rpc/human/human_death.h"
#include "world/client.h"

#include "sdk/mafia/ui/c_game_gui_2_module.h"
#include <logging/logger.h>

typedef void(__fastcall *C_Human2__SetupDeath_t)(SDK::C_Human2 *_this, void *);
C_Human2__SetupDeath_t C_Human2__SetupDeath_original = nullptr;
void __fastcall C_Human2__SetupDeath(SDK::C_Human2 *pThis, void *entityMsgDamage) {
    // Is the local player ?
    if (pThis == MafiaMP::Game::Helpers::Controls::GetLocalPlayer()) {
        Framework::Logging::GetLogger("Hooks")->debug("LocalPlayer just died");

        SDK::ue::C_CntPtr<uintptr_t> syncObject;
        SDK::mafia::ui::GetGameGui2Module()->GetFader()->FadeIn(syncObject, 0.5, "DeathScreen", true);

        // If the local player is in a car, we just get him out first
        SDK::C_Car *currentCar = pThis ? (SDK::C_Car *)pThis->GetOwner() : nullptr;
        if (currentCar) {
            SDK::C_Actor *someActor = *(SDK::C_Actor **)((uint64_t)currentCar + 0xA8);
            pThis->GetCharacterController()->TriggerActorAction(someActor, SDK::E_AA_LEAVE_CAR, 0, true, false);
        }

        FW_SEND_CLIENT_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::HumanDeath, MafiaMP::Core::gApplication->GetLocalPlayer());
        return;
    }

    // If not we just process as intended
    return C_Human2__SetupDeath_original(pThis, entityMsgDamage);
}

static InitFunction init([]() {
    // Hook the local player death so we can actually respawn it without the blue screen of the death
    const auto C_Human2__SetupDeath_addr = hook::pattern("48 8B C4 55 56 41 56 48 8D 68 ? 48 81 EC ? ? ? ? C7 45 ? ? ? ? ?").get_first();
    MH_CreateHook((LPVOID)C_Human2__SetupDeath_addr, (PBYTE)C_Human2__SetupDeath, reinterpret_cast<void **>(&C_Human2__SetupDeath_original));

    // Make sure C_AICommand_AimAt::C_AICommand_AimAt always try to use position and not entity
    // TODO make it pattern based
    hook::nop(0x0000001427DC07C, 0x4F);
    },
    "Human");
