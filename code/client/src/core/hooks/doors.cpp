#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include <atomic>

#include "core/world_settings.h"

// Doors in Mafia DE are scene objects carrying a door component. A runtime door starts locked or
// out of the player's reach as its location's mission set it up; outside those missions nothing
// opens it again. With every door open, each one is unlocked and enabled for the player as it
// activates, as a mission script would.
namespace MafiaMP::Core::Doors {
    std::atomic_bool gAllOpen {false};

    void SetAllOpen(bool open) {
        gAllOpen = open;
    }
} // namespace MafiaMP::Core::Doors

namespace {
    using DoorFn_t                                     = void(__fastcall *)(void *);
    DoorFn_t C_RuntimeDoorComponent__Activate_original = nullptr;
    DoorFn_t C_RuntimeDoorComponent__EnableForPlayer   = nullptr;
    DoorFn_t C_RuntimeDoorComponent__Unlock            = nullptr;

    void __fastcall C_RuntimeDoorComponent__Activate(void *door) {
        C_RuntimeDoorComponent__Activate_original(door);
        if (MafiaMP::Core::Doors::gAllOpen) {
            C_RuntimeDoorComponent__EnableForPlayer(door);
            C_RuntimeDoorComponent__Unlock(door);
        }
    }
} // namespace

static InitFunction init(
    []() {
        C_RuntimeDoorComponent__EnableForPlayer = hook::get_pattern<std::remove_pointer_t<DoorFn_t>>("C6 81 34 01 00 00 01 E9");
        C_RuntimeDoorComponent__Unlock          = hook::get_pattern<std::remove_pointer_t<DoorFn_t>>("C6 81 36 01 00 00 00 E9");
        const auto activate                     = hook::get_pattern("48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 50 48 8B F9 E8");
        MH_CreateHook(activate, reinterpret_cast<LPVOID>(C_RuntimeDoorComponent__Activate), reinterpret_cast<void **>(&C_RuntimeDoorComponent__Activate_original));
    },
    "Doors");
