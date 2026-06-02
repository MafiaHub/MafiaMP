#include <utils/safe_win32.h>

#include <MinHook.h>

#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include "core/application.h"

#include "sdk/c_inventory_wrapper.h"
#include "sdk/entities/c_player_2.h"
#include "sdk/entities/human/c_human_weapon_controller.h"
#include "sdk/ue/game/humanai/c_character_state_handler_aim.h"
#include "sdk/ue/sys/math/c_vector.h"

#include "game/helpers/controls.h"

#include <core/modules/human.h>

#include <core_modules.h>
#include <networking/replication/replication_manager.h>
#include <world/engine.h>

#include <logging/logger.h>

using namespace SDK;

// Resolve the ClientHuman whose game ped uses this weapon controller.
static MafiaMP::Core::Modules::Human *FindHumanByHumanWeaponController(SDK::C_HumanWeaponController *controller) {
    auto *world = Framework::CoreModules::GetWorldEngine();
    auto *repl  = world ? world->GetReplication() : nullptr;
    if (!repl || !controller) {
        return nullptr;
    }
    MafiaMP::Core::Modules::Human *found = nullptr;
    repl->ForEachEntity([&](Framework::Networking::Replication::NetworkEntity *e) {
        if (found) {
            return;
        }
        if (auto *human = dynamic_cast<MafiaMP::Core::Modules::Human *>(e); human && human->human && human->human->GetHumanWeaponController() == controller) {
            found = human;
        }
    });
    return found;
}

typedef bool(__fastcall *C_HumanWeaponController__SetAiming_t)(SDK::C_HumanWeaponController *, bool);
C_HumanWeaponController__SetAiming_t C_HumanWeaponController__SetAiming_Original = nullptr;
bool C_HumanWeaponController__SetAiming(SDK::C_HumanWeaponController *_this, bool aiming) {
    auto gameLocalPlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();
    if (gameLocalPlayer && gameLocalPlayer->GetHumanWeaponController() == _this) {
        if (auto *localPlayer = MafiaMP::Core::gApplication->GetLocalPlayer()) {
            localPlayer->data.weaponData.isAiming = aiming;
        }
    }
    else if (auto *remoteHuman = FindHumanByHumanWeaponController(_this)) {
        aiming = remoteHuman->data.weaponData.isAiming;
        Framework::Logging::GetLogger("Hooks")->info("Remote ped aiming {}", aiming);
    }

    return C_HumanWeaponController__SetAiming_Original(_this, aiming);
}

typedef bool(__fastcall *C_HumanWeaponController__SetFirePressedFlag_t)(SDK::C_HumanWeaponController *, bool);
C_HumanWeaponController__SetFirePressedFlag_t C_HumanWeaponController__SetFirePressedFlag_Original = nullptr;
bool C_HumanWeaponController__SetFirePressedFlag(SDK::C_HumanWeaponController *_this, bool firePressed) {
    auto gameLocalPlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();
    if (gameLocalPlayer && gameLocalPlayer->GetHumanWeaponController() == _this) {
        if (auto *localPlayer = MafiaMP::Core::gApplication->GetLocalPlayer()) {
            localPlayer->data.weaponData.isFiring = firePressed;
        }
    }
    else if (auto *remoteHuman = FindHumanByHumanWeaponController(_this)) {
        firePressed = remoteHuman->data.weaponData.isFiring;
    }

    return C_HumanWeaponController__SetFirePressedFlag_Original(_this, firePressed);
}

typedef void(__fastcall *C_HumanWeaponController__GetShotPosDir_t)(SDK::C_HumanWeaponController *, ue::sys::math::C_Vector &, ue::sys::math::C_Vector &, float *);
C_HumanWeaponController__GetShotPosDir_t C_HumanWeaponController__GetShotPosDir_Original = nullptr;
void C_HumanWeaponController__GetShotPosDir(SDK::C_HumanWeaponController *_this, ue::sys::math::C_Vector &pos, ue::sys::math::C_Vector &dir, float *distance) {
    auto gameLocalPlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();
    if (gameLocalPlayer && gameLocalPlayer->GetHumanWeaponController() == _this) {
        if (auto *localPlayer = MafiaMP::Core::gApplication->GetLocalPlayer()) {
            C_HumanWeaponController__GetShotPosDir_Original(_this, pos, dir, distance);
            localPlayer->data.weaponData.aimPos = {pos.x, pos.y, pos.z};
            localPlayer->data.weaponData.aimDir = {dir.x, dir.y, dir.z};
        }
    }
    else if (auto *remoteHuman = FindHumanByHumanWeaponController(_this)) {
        // For a remote ped we feed back the broadcast aim instead of querying the game.
        pos = {remoteHuman->data.weaponData.aimPos.x, remoteHuman->data.weaponData.aimPos.y, remoteHuman->data.weaponData.aimPos.z};
        dir = {remoteHuman->data.weaponData.aimDir.x, remoteHuman->data.weaponData.aimDir.y, remoteHuman->data.weaponData.aimDir.z};
    }
}

typedef bool(__fastcall *C_HumanWeaponController__DoWeaponSelectByItemId_t)(void *, unsigned int, bool);
C_HumanWeaponController__DoWeaponSelectByItemId_t C_HumanWeaponController__DoWeaponSelectByItemId_Original = nullptr;
bool C_HumanWeaponController__DoWeaponSelectByItemId(SDK::C_HumanWeaponController *_this, unsigned int itemId, bool unk1) {
    auto gameLocalPlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();
    if (gameLocalPlayer && gameLocalPlayer->GetHumanWeaponController() == _this) {
        if (auto *localPlayer = MafiaMP::Core::gApplication->GetLocalPlayer()) {
            localPlayer->data.weaponData.currentWeaponId = uint16_t(itemId);
        }
    }

    return C_HumanWeaponController__DoWeaponSelectByItemId_Original(_this, itemId, unk1);
}

typedef bool(__fastcall *C_HumanWeaponController_DoShot_t)(SDK::C_HumanWeaponController *, void *, ue::sys::math::C_Vector const *, ue::sys::math::C_Vector const *, bool, bool);
C_HumanWeaponController_DoShot_t C_HumanWeaponController_DoShot_original = nullptr;
bool C_HumanWeaponController_DoShot(SDK::C_HumanWeaponController *pThis, void *unk, ue::sys::math::C_Vector const *pos1, ue::sys::math::C_Vector const *pos2, bool unk1, bool unk2) {
    auto gameLocalPlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();
    if (gameLocalPlayer && gameLocalPlayer->GetHumanWeaponController() == pThis) {
        const glm::vec3 pos = pos1 ? glm::vec3 {pos1->x, pos1->y, pos1->z} : glm::vec3 {};
        const glm::vec3 dir = pos2 ? glm::vec3 {pos2->x, pos2->y, pos2->z} : glm::vec3 {};
        MafiaMP::Core::gApplication->GetLocalPlayerEvents().Shot(pos, dir, unk1, unk2);
    }

    return C_HumanWeaponController_DoShot_original(pThis, unk, pos1, pos2, unk1, unk2);
}

typedef void(__fastcall *C_HumanWeaponController__DoWeaponReloadInventory_t)(SDK::C_HumanWeaponController *, int);
C_HumanWeaponController__DoWeaponReloadInventory_t C_HumanWeaponController__DoWeaponReloadInventory_original = nullptr;
void C_HumanWeaponController__DoWeaponReloadInventory(SDK::C_HumanWeaponController *pThis, int unk) {
    auto gameLocalPlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();
    if (gameLocalPlayer && gameLocalPlayer->GetHumanWeaponController() == pThis) {
        MafiaMP::Core::gApplication->GetLocalPlayerEvents().Reloaded(unk);
    }

    C_HumanWeaponController__DoWeaponReloadInventory_original(pThis, unk);
}

typedef bool(__fastcall *C_HumanInventory__CanFire_t)(SDK::C_HumanInventory *);
C_HumanInventory__CanFire_t C_HumanInventory__CanFire_original = nullptr;
bool C_HumanInventory__CanFire(SDK::C_HumanInventory *pThis) {
    auto gameLocalPlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();

    // In case it's the local player, normal behavior
    if (gameLocalPlayer && gameLocalPlayer->GetHumanInventory() == pThis) {
        return C_HumanInventory__CanFire_original(pThis);
    }

    // If it's a remote ped, always allows to shot
    return true;
}

static InitFunction init([]() {
    // Hook the methods
    const auto addr1 = hook::get_opcode_address("E8 ? ? ? ? 48 8B 43 10 48 8B 88 ? ? ? ? C6 41 14 00");
    // MH_CreateHook((LPVOID)addr1, (PBYTE)C_HumanWeaponController__SetAiming, reinterpret_cast<void **>(&C_HumanWeaponController__SetAiming_Original));

    const auto addr2 = hook::pattern("84 D2 75 11 33 C0").get_first();
    // MH_CreateHook((LPVOID)addr2, (PBYTE)C_HumanWeaponController__SetFirePressedFlag, reinterpret_cast<void **>(&C_HumanWeaponController__SetFirePressedFlag_Original));

    const auto addr3 = hook::pattern("4C 8B DC 49 89 5B 08 49 89 73 10 49 89 7B 18 4D 89 73 20 55").get_first();
    // MH_CreateHook((LPVOID)addr3, (PBYTE)C_HumanWeaponController__GetShotPosDir, reinterpret_cast<void **>(&C_HumanWeaponController__GetShotPosDir_Original));

    const auto addr4 = hook::pattern("48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 40 48 8B 81 60").get_first();
    MH_CreateHook((LPVOID)addr4, (PBYTE)C_HumanWeaponController__DoWeaponSelectByItemId, reinterpret_cast<void **>(&C_HumanWeaponController__DoWeaponSelectByItemId_Original));

    MH_CreateHook((LPVOID)gPatterns.C_HumanWeaponController__DoShot, (PBYTE)C_HumanWeaponController_DoShot, reinterpret_cast<void **>(&C_HumanWeaponController_DoShot_original));
    MH_CreateHook((LPVOID)gPatterns.C_HumanWeaponController__DoWeaponReloadInventory, (PBYTE)C_HumanWeaponController__DoWeaponReloadInventory, reinterpret_cast<void **>(&C_HumanWeaponController__DoWeaponReloadInventory_original));

    const auto C_HumanInventory__CanFire_Addr = hook::get_opcode_address("E8 ? ? ? ? 84 C0 75 2E F6 83 ? ? ? ? ?");
    MH_CreateHook((LPVOID)C_HumanInventory__CanFire_Addr, (PBYTE)C_HumanInventory__CanFire, reinterpret_cast<void **>(&C_HumanInventory__CanFire_original));
    },
    "WeaponController");
