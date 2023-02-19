#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include "../../sdk/entities/c_actor.h"
#include "../../sdk/entities/c_car.h"
#include "../../sdk/ue/game/humainai/c_character_controller.h"
#include "../../sdk/entities/c_player_2.h"
#include "../../sdk/entities/c_human_2.h"
#include "../../sdk/entities//human/c_human_script.h"
#include "../../sdk/c_player_teleport_module.h"
#include "../../game/helpers/controls.h"

#include <logging/logger.h>

typedef void(__fastcall *C_Human2__SetupDeath_t)(SDK::C_Human2 *_this, void*);
C_Human2__SetupDeath_t C_Human2__SetupDeath_original = nullptr;
void __fastcall C_Human2__SetupDeath(SDK::C_Human2 *pThis, void *entityMsgDamage) {
    // Is the local player ?
    if (pThis == MafiaMP::Game::Helpers::Controls::GetLocalPlayer()) {
        Framework::Logging::GetLogger("Hooks")->debug("LocalPlayer just died");

        // If the local player is in a car, we just get him out first
        SDK::C_Car *currentCar = pThis ? (SDK::C_Car *)pThis->GetOwner() : nullptr;
        if (currentCar) {
            SDK::C_Actor *someActor = *(SDK::C_Actor **)((uint64_t)currentCar + 0xA8);
            pThis->GetCharacterController()->TriggerActorAction(someActor, SDK::E_AA_LEAVE_CAR, 0, true, false);
        }

        // For now we just respawn the player at a given position - NOTE this should be handled by server
        SDK::ue::sys::math::C_Vector pos = {-934.0f, -228.859f, 5.004f};
        SDK::ue::sys::math::C_Vector dir = {0.0f, 0.0f, 0.0f};

        // Create a dummy sync object
        uint64_t syncObject;

        //Teleport the player and restore the city around - Broken for now
        // SDK::C_PlayerTeleportModule::GetInstance()->TeleportPlayer(&syncObject, pos, dir, false, true, false);

        // Restore player health to max
        pThis->GetHumanScript()->SetHealth(pThis->GetHumanScript()->GetHealthMax());

        // Broadcast network event
        // TODO: implement
    }

    // If not we just process as intended
    return C_Human2__SetupDeath_original(pThis, entityMsgDamage);
}

static InitFunction init([]() {
    // Hook the local player death so we can actually respawn it without the blue screen of the death
    const auto C_Human2__SetupDeath_addr = hook::pattern("48 8B C4 55 56 41 56 48 8D 68 ? 48 81 EC ? ? ? ? C7 45 ? ? ? ? ?").get_first();
    MH_CreateHook((LPVOID)C_Human2__SetupDeath_addr, (PBYTE)C_Human2__SetupDeath, reinterpret_cast<void **>(&C_Human2__SetupDeath_original));
});
