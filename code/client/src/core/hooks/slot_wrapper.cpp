#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include "../../sdk/ue/sys/core/c_frame.h"
#include "../../sdk/ue/sys/core/c_scene.h"
#include "../../sdk/ue/sys/math/c_vector.h"

#include "../../sdk/mafia/streaming/c_slot_wrapper.h"

#include "../../sdk/entities/c_actor.h"
#include "../../sdk/ue/sys/utils/c_hash_name.h"

typedef bool(__fastcall *C_SlotManagerWrapper__ConnectToQuota_t)(void *, SDK::mafia::streaming::I_SlotWrapper *, char const *, int, int);
C_SlotManagerWrapper__ConnectToQuota_t C_SlotManagerWrapper__ConnectToQuota_original = nullptr;

bool C_SlotManagerWrapper__ConnectToQuota(void* pThis, SDK::mafia::streaming::I_SlotWrapper* slotWrapper, char const* name, int slotType, int unk) {
    // Framework::Logging::GetLogger("Hooks")->debug("[Wrapper] Connected to quota {} for slot type {} and unk {} ({})", name, slotType, unk, slotWrapper->GetWrapperName());
    return C_SlotManagerWrapper__ConnectToQuota_original(pThis, slotWrapper, name, slotType, unk);
}

typedef int64_t(__fastcall *C_SlotManagerWrapper__LoadData_t)(void *, char const *, SDK::ue::sys::core::C_Scene *, unsigned int, char const *, bool *, bool);
C_SlotManagerWrapper__LoadData_t C_SlotManagerWrapper__LoadData_original = nullptr;

int64_t C_SlotManagerWrapper__LoadData(void* pThis, char const* sdsName, SDK::ue::sys::core::C_Scene* scene, unsigned int unk, char const* dataName, bool* unk2, bool unk3) {
    // Framework::Logging::GetLogger("Hooks")->debug("[Wrapper] Loaded data = {} | {} | {}", sdsName, unk, dataName);
    return C_SlotManagerWrapper__LoadData_original(pThis, sdsName, scene, unk, dataName, unk2, unk3);
}

typedef int64_t(__fastcall *C_Slot__LoadData_t)(void *, char const *, SDK::ue::sys::core::C_Scene *, unsigned int, void *, char const *, bool *, bool);
C_Slot__LoadData_t C_Slot__LoadData_original = nullptr;

int64_t C_Slot__LoadData(void *pThis, char const *sdsName, SDK::ue::sys::core::C_Scene *scene, unsigned int unk, void * unkPtr, char const *dataName, bool *unk2, bool unk3) {
    // Framework::Logging::GetLogger("Hooks")->debug("Loaded data = {} | {} | {}", sdsName, unk, dataName);
    return C_Slot__LoadData_original(pThis, sdsName, scene, unk, unkPtr, dataName, unk2, unk3);
}

static InitFunction init([]() {
    const auto C_SlotManagerWrapper__ConnectToQuota_Addr = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 70 FF 81 ? ? ? ?").get_first());
    MH_CreateHook((LPVOID)C_SlotManagerWrapper__ConnectToQuota_Addr, (PBYTE)C_SlotManagerWrapper__ConnectToQuota, reinterpret_cast<void **>(&C_SlotManagerWrapper__ConnectToQuota_original));

    const auto C_SlotManagerWrapper__LoadData_Addr = hook::get_opcode_address("E8 ? ? ? ? 84 C0 75 80");
    MH_CreateHook((LPVOID)C_SlotManagerWrapper__LoadData_Addr, (PBYTE)C_SlotManagerWrapper__LoadData, reinterpret_cast<void **>(&C_SlotManagerWrapper__LoadData_original));

    const auto C_Slot__LoadData_Addr = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4C 24 ? 0F B6 D8 48 85 C9 74 0A");
    MH_CreateHook((LPVOID)C_Slot__LoadData_Addr, (PBYTE)C_Slot__LoadData, reinterpret_cast<void **>(&C_Slot__LoadData_original));
});
