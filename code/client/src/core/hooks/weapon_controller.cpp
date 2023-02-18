#include <utils/safe_win32.h>
#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include "core/application.h"
#include "shared/modules/human_sync.hpp"
#include "../../sdk/ue/sys/math/c_vector.h"
#include "../../sdk/entities/c_player_2.h"

#include "game/helpers/controls.h"

#include <world/client.h>
#include "shared/game_rpc/human/human_shoot.h"
#include "shared/game_rpc/human/human_reload.h"

#include <logging/logger.h>
#include <flecs/flecs.h>
#include <core/modules/human.h>

using namespace SDK;

flecs::entity FindHumanByHumanWeaponController(void* C_HumanWeaponController) {
    flecs::entity foundPlayerEntity {};
    MafiaMP::Core::Modules::Human::findAllHumans.each([C_HumanWeaponController, &foundPlayerEntity](flecs::entity e, MafiaMP::Core::Modules::Human::Tracking &tracking) {
        if (tracking.human->GetHumanWeaponController() == C_HumanWeaponController) {
            foundPlayerEntity = e;
            return;
        }
    });

    return foundPlayerEntity;
}

typedef bool(__fastcall *C_HumanWeaponController__SetAiming_t)(void *, bool);
C_HumanWeaponController__SetAiming_t C_HumanWeaponController__SetAiming_Original = nullptr;
bool C_HumanWeaponController__SetAiming(void* _this, bool aiming) {
    auto gameLocalPlayer   = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();
    if (gameLocalPlayer && gameLocalPlayer->GetHumanWeaponController() == _this) {
        if (const auto localPlayer = MafiaMP::Core::gApplication->GetLocalPlayer()) {
            auto updateData                 = localPlayer.get_mut<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
            updateData->weaponData.isAiming = aiming;
        }
    }
    /*else if (const auto remoteHuman = FindHumanByHumanWeaponController(_this)) {
        const auto updateData = remoteHuman.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        aiming                = updateData->weaponData.isAiming;
    }*/

    return C_HumanWeaponController__SetAiming_Original(_this, aiming);
}

typedef bool(__fastcall *C_HumanWeaponController__SetFirePressedFlag_t)(void *, bool);
C_HumanWeaponController__SetFirePressedFlag_t C_HumanWeaponController__SetFirePressedFlag_Original = nullptr;
bool C_HumanWeaponController__SetFirePressedFlag(void *_this, bool firePressed) {
    auto gameLocalPlayer   = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();
    if (gameLocalPlayer && gameLocalPlayer->GetHumanWeaponController() == _this) {
        if (const auto localPlayer = MafiaMP::Core::gApplication->GetLocalPlayer()) {
            // auto updateData                 = localPlayer.get_mut<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
            // updateData->weaponData.isFiring = firePressed;
        }
    } /*
    else if (const auto remoteHuman = FindHumanByHumanWeaponController(_this)) {
        const auto updateData = remoteHuman.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        firePressed           = updateData->weaponData.isFiring;
    }*/

    return C_HumanWeaponController__SetFirePressedFlag_Original(_this, firePressed);
}

typedef void(__fastcall *C_HumanWeaponController__GetShotPosDir_t)(void *, ue::sys::math::C_Vector &, ue::sys::math::C_Vector &, float*);
C_HumanWeaponController__GetShotPosDir_t C_HumanWeaponController__GetShotPosDir_Original = nullptr;
void C_HumanWeaponController__GetShotPosDir(void *_this, ue::sys::math::C_Vector &pos, ue::sys::math::C_Vector &dir, float *distance) {
    C_HumanWeaponController__GetShotPosDir_Original(_this, pos, dir, distance);

    if (!distance)
        return;

    auto gameLocalPlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();
    if (gameLocalPlayer && gameLocalPlayer->GetHumanWeaponController() == _this) {
        if (const auto localPlayer = MafiaMP::Core::gApplication->GetLocalPlayer()) {
            auto updateData               = localPlayer.get_mut<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
            updateData->weaponData.aimPos = {pos.x, pos.y, pos.z};
            updateData->weaponData.aimDir = {dir.x, dir.y, dir.z};
        }
    } else if (auto remoteHuman = FindHumanByHumanWeaponController(_this)) {
        const auto updateData = remoteHuman.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        pos                   = {updateData->weaponData.aimPos.x, updateData->weaponData.aimPos.y, updateData->weaponData.aimPos.z};
        dir                   = {updateData->weaponData.aimDir.x, updateData->weaponData.aimDir.y, updateData->weaponData.aimDir.z};
    }
}

typedef bool(__fastcall *C_HumanWeaponController__DoWeaponSelectByItemId_t)(void*, unsigned int, bool);
C_HumanWeaponController__DoWeaponSelectByItemId_t C_HumanWeaponController__DoWeaponSelectByItemId_Original = nullptr;
bool C_HumanWeaponController__DoWeaponSelectByItemId(void *_this, unsigned int itemId, bool unk1) {
    auto gameLocalPlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();
    if (gameLocalPlayer && gameLocalPlayer->GetHumanWeaponController() == _this) {
        if (const auto localPlayer = MafiaMP::Core::gApplication->GetLocalPlayer()) {
            auto updateData                 = localPlayer.get_mut<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
            updateData->weaponData.currentWeaponId = uint16_t(itemId);
        }
    }

    return C_HumanWeaponController__DoWeaponSelectByItemId_Original(_this, itemId, unk1);
}

typedef bool(__fastcall *C_HumanWeaponController_DoShot_t)(void *, void *, ue::sys::math::C_Vector const *, ue::sys::math::C_Vector const *, bool, bool);
C_HumanWeaponController_DoShot_t C_HumanWeaponController_DoShot_original = nullptr;
bool C_HumanWeaponController_DoShot(void *pThis, void *unk, ue::sys::math::C_Vector const *pos1, ue::sys::math::C_Vector const *pos2, bool unk1, bool unk2){
    auto gameLocalPlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();

    // In case it's the local player, send an rpc
    if (gameLocalPlayer && gameLocalPlayer->GetHumanWeaponController() == pThis) {
        const auto playerID = MafiaMP::Core::gApplication->GetLocalPlayer();
        glm::vec3 pos;
        if (pos1)
            pos = {pos1->x, pos1->y, pos1->z};
        glm::vec3 dir;
        if (pos2)
            dir = {pos2->x, pos2->y, pos2->z};
        FW_SEND_CLIENT_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::HumanShoot, playerID, pos, dir, unk1, unk2);
    }

    return C_HumanWeaponController_DoShot_original(pThis, unk, pos1, pos2, unk1, unk2);
}

typedef void(__fastcall *C_HumanWeaponController__DoWeaponReloadInventory_t)(void *, int);
C_HumanWeaponController__DoWeaponReloadInventory_t C_HumanWeaponController__DoWeaponReloadInventory_original = nullptr;
void C_HumanWeaponController__DoWeaponReloadInventory(void *pThis, int unk) {
    auto gameLocalPlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();

    // In case it's the local player, send an rpc
    if (gameLocalPlayer && gameLocalPlayer->GetHumanWeaponController() == pThis) {
        const auto playerID = MafiaMP::Core::gApplication->GetLocalPlayer();
        FW_SEND_CLIENT_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::HumanReload, playerID, unk);
    }

    C_HumanWeaponController__DoWeaponReloadInventory_original(pThis, unk);
}

typedef bool(__fastcall *C_HumanInventory__CanFire_t)(void *);
C_HumanInventory__CanFire_t C_HumanInventory__CanFire_original = nullptr;
bool C_HumanInventory__CanFire(void* pThis) {
    auto gameLocalPlayer = MafiaMP::Game::Helpers::Controls::GetLocalPlayer();

    // In case it's the local player, normal behavior
    if (gameLocalPlayer && gameLocalPlayer->GetInventoryWrapper() == pThis) {
        return C_HumanInventory__CanFire_original(pThis);
    }

    // If it's a remote ped, always allows to shot
    return true;
}

static InitFunction init([]() {
    const auto addr1 = hook::get_opcode_address("E8 ? ? ? ? 48 8B 43 10 48 8B 88 ? ? ? ? C6 41 14 00");
    MH_CreateHook((LPVOID)addr1, (PBYTE)C_HumanWeaponController__SetAiming, reinterpret_cast<void **>(&C_HumanWeaponController__SetAiming_Original));

    const auto addr2 = hook::pattern("84 D2 75 11 33 C0").get_first();
    MH_CreateHook((LPVOID)addr2, (PBYTE)C_HumanWeaponController__SetFirePressedFlag, reinterpret_cast<void **>(&C_HumanWeaponController__SetFirePressedFlag_Original));

    const auto addr3 = hook::pattern("4C 8B DC 49 89 5B 08 49 89 73 10 49 89 7B 18 4D 89 73 20 55").get_first();
    MH_CreateHook((LPVOID)addr3, (PBYTE)C_HumanWeaponController__GetShotPosDir, reinterpret_cast<void **>(&C_HumanWeaponController__GetShotPosDir_Original));

    const auto addr4 = hook::pattern("48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 40 48 8B 81 60").get_first();
    MH_CreateHook((LPVOID)addr4, (PBYTE)C_HumanWeaponController__DoWeaponSelectByItemId, reinterpret_cast<void **>(&C_HumanWeaponController__DoWeaponSelectByItemId_Original));

    MH_CreateHook((LPVOID)gPatterns.C_HumanWeaponController__DoShot, (PBYTE)C_HumanWeaponController_DoShot, reinterpret_cast<void **>(&C_HumanWeaponController_DoShot_original));
    MH_CreateHook((LPVOID)gPatterns.C_HumanWeaponController__DoWeaponReloadInventory, (PBYTE)C_HumanWeaponController__DoWeaponReloadInventory, reinterpret_cast<void **>(&C_HumanWeaponController__DoWeaponReloadInventory_original));

    const auto C_HumanInventory__CanFire_Addr = hook::get_opcode_address("E8 ? ? ? ? 84 C0 75 2E F6 83 ? ? ? ? ?");
    MH_CreateHook((LPVOID)C_HumanInventory__CanFire_Addr, (PBYTE)C_HumanInventory__CanFire, reinterpret_cast<void **>(&C_HumanInventory__CanFire_original));
});
