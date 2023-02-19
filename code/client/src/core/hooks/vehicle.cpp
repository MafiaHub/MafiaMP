#include <utils/safe_win32.h>
#include <MinHook.h>

#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include "../modules/vehicle.h"
#include "shared/modules/vehicle_sync.hpp"

#include "../../sdk/entities/c_actor.h"
#include "../../sdk/entities/c_car.h"
#include "../../sdk/entities/c_human_2.h"
#include "../../sdk/entities/c_vehicle.h"
#include "../../sdk/wrappers/c_human_2_car_wrapper.h"

SDK::C_Actor* C_ActorAction__GetOwnerAsActor(void *pThis) {
    return hook::this_call<SDK::C_Actor *>(0x0000001423434E0, pThis);
}

typedef bool(__fastcall *C_CarActionEnter__TestActionInternal_t)(void *, SDK::C_Actor *, bool);
C_CarActionEnter__TestActionInternal_t C_CarActionEnter__TestActionInternal_original = nullptr;
bool C_CarActionEnter__TestActionInternal(void* pThis, SDK::C_Actor* actor, bool locationCheck) {
    const auto actionActor = C_ActorAction__GetOwnerAsActor(pThis);
    const auto vehicle     = MafiaMP::Core::Modules::Vehicle::GetCarEntity(reinterpret_cast<SDK::C_Car *>(actionActor));
    if (!vehicle) {
        return true;
    }

    const auto carData = vehicle.get<MafiaMP::Shared::Modules::VehicleSync::UpdateData>();
    if (!carData) {
        return true;
    }
    return carData->locked == MafiaMP::Shared::Modules::VehicleSync::LockState::Unlocked;
}

typedef bool(__fastcall *C_CarActionBreakIn__TestActionInternal_t)(void *, SDK::C_Actor *, bool);
C_CarActionBreakIn__TestActionInternal_t C_CarActionBreakIn__TestActionInternal_original = nullptr;
bool C_CarActionBreakIn__TestActionInternal(void *pThis, SDK::C_Actor *actor, bool locationCheck) {
    const auto actionActor = C_ActorAction__GetOwnerAsActor(pThis);
    const auto vehicle     = MafiaMP::Core::Modules::Vehicle::GetCarEntity(reinterpret_cast<SDK::C_Car *>(actionActor));
    if (!vehicle) {
        return true;
    }

    const auto carData = vehicle.get<MafiaMP::Shared::Modules::VehicleSync::UpdateData>();
    if (!carData) {
        return true;
    }
    return carData->locked == MafiaMP::Shared::Modules::VehicleSync::LockState::Breakable;
}

typedef bool(__fastcall *C_CarActionLeave__TestAction_t)(void *, SDK::C_Actor *);
C_CarActionLeave__TestAction_t C_CarActionLeave__TestAction_original = nullptr;
bool C_CarActionLeave__TestAction(void *pThis, SDK::C_Actor *actor) {
    const auto actionActor = C_ActorAction__GetOwnerAsActor(pThis);
    const auto vehicle     = MafiaMP::Core::Modules::Vehicle::GetCarEntity(reinterpret_cast<SDK::C_Car *>(actionActor));
    if (!vehicle) {
        return true;
    }

    const auto carData = vehicle.get<MafiaMP::Shared::Modules::VehicleSync::UpdateData>();
    if (!carData) {
        return true;
    }
    return carData->locked == MafiaMP::Shared::Modules::VehicleSync::LockState::Unlocked;
}

typedef void(__fastcall *C_Human2CarWrapper__StartDrive_t)(SDK::C_Human2CarWrapper *, SDK::C_Actor *, bool);
C_Human2CarWrapper__StartDrive_t C_Human2CarWrapper__StartDrive_original = nullptr;
void C_Human2CarWrapper__StartDrive(SDK::C_Human2CarWrapper *pThis, SDK::C_Actor *pActor, bool unk) {
    const auto seatID = pThis->GetSeatID(pActor);
    if (seatID != 999) {
        pThis->m_pUsedCar->SetSeatStatus(reinterpret_cast<SDK::I_Human2 *>(pActor), seatID, SDK::S_BaseSeat::E_BaseSeatStatus::OCCUPIED);
        reinterpret_cast<SDK::C_Human2 *>(pActor)->EnableShadows(false);
        reinterpret_cast<SDK::C_Human2 *>(pActor)->EnableHumanClothes();
    }
}
typedef void(__fastcall *C_Human2CarWrapper__EndDrive_t)(SDK::C_Human2CarWrapper *, SDK::C_Actor *, bool, bool);
C_Human2CarWrapper__EndDrive_t C_Human2CarWrapper__EndDrive_original = nullptr;
void C_Human2CarWrapper__EndDrive(SDK::C_Human2CarWrapper *pThis, SDK::C_Actor *pActor, bool unk, bool unk2) {
    const auto seatID = pThis->GetSeatID(pActor);
    if (seatID != 999) {
        pThis->m_pUsedCar->SetSeatStatus(reinterpret_cast<SDK::I_Human2 *>(pActor), seatID, SDK::S_BaseSeat::E_BaseSeatStatus::EMPTY);
        reinterpret_cast<SDK::C_Human2 *>(pActor)->EnableShadows(true);
        reinterpret_cast<SDK::C_Human2 *>(pActor)->EnableHumanClothes();
    }
}

static InitFunction init([]() {    
    // Disable automated vehicle enable (engine, siren, beacon lights etc...) when player enter it
    const auto C_Human2CarWrapper__StartDrive_Addr = hook::pattern("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 57 48 83 EC 70 45 33 FF").get_first();
    MH_CreateHook((LPVOID)C_Human2CarWrapper__StartDrive_Addr, (PBYTE)C_Human2CarWrapper__StartDrive, reinterpret_cast<void **>(&C_Human2CarWrapper__StartDrive_original));

    // Hook the game EndDrive method so we can avoid it disabling the vehicle (engine, lights etc...)
    const auto C_Human2CarWrapper__EndDrive_Addr = hook::get_opcode_address("E8 ? ? ? ? 48 8B 43 08 48 8B 7C 24 ?");
    MH_CreateHook((LPVOID)C_Human2CarWrapper__EndDrive_Addr, (PBYTE)C_Human2CarWrapper__EndDrive, reinterpret_cast<void **>(&C_Human2CarWrapper__EndDrive_original));

    const auto C_CarActionEnter__TestActionInternal_Addr = hook::pattern("40 53 55 57 41 54 41 55 48 83 EC 40 41 0F B6 F8").get_first();
    // MH_CreateHook((LPVOID)C_CarActionEnter__TestActionInternal_Addr, (PBYTE)C_CarActionEnter__TestActionInternal, reinterpret_cast<void **>(&C_CarActionEnter__TestActionInternal_original));

    const auto C_CarActionBreakIn__TestActionBreakIn_Addr = hook::pattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 30 45 0F B6 F8").get_first();
    // MH_CreateHook((LPVOID)C_CarActionBreakIn__TestActionBreakIn_Addr, (PBYTE)C_CarActionBreakIn__TestActionInternal, reinterpret_cast<void **>(&C_CarActionBreakIn__TestActionInternal_original));

    const auto C_CarActionLeave__TestAction_Addr = hook::pattern("40 53 48 83 EC 20 48 8B DA E8 ? ? ? ? 48 8B C8 4C 8B 00 41 FF 90 ? ? ? ? 48 3B D8 75 29 E8 ? ? ? ? 48 8B C8 48 8B 10 FF 92 ? ? ? ? 48 8B C8 BA ? ? ? ? E8 ? ? ? ? 84 C0 0F 94 C0 48 83 C4 20 5B C3 B0 01 48 83 C4 20 5B C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 41 B0 01 E9 ? ? ? ? CC CC CC CC CC CC CC CC 48 89 5C 24 ? 48 89 6C 24 ?").get_first();
    //MH_CreateHook((LPVOID)C_CarActionLeave__TestAction_Addr, (PBYTE)C_CarActionLeave__TestAction, reinterpret_cast<void **>(&C_CarActionLeave__TestAction_original));
});
