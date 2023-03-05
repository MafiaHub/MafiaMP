#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include "../../sdk/ue/sys/math/c_vector.h"

#include "../../sdk/mafia/streaming/c_slot_wrapper.h"

typedef bool(__fastcall *C_SlotManager__ConnectToQuota_t)(void *, SDK::mafia::streaming::I_SlotWrapper *, char const *, int, int);
C_SlotManager__ConnectToQuota_t C_SlotManager__ConnectToQuota_original = nullptr;

bool C_SlotManager__ConnectToQuota(void* pThis, SDK::mafia::streaming::I_SlotWrapper* slotWrapper, char const* name, int slotType, int unk) {
    Framework::Logging::GetLogger("Hooks")->debug("Connected to quota {} for slot type {} and unk {} ({})", name, slotType, unk, slotWrapper->GetWrapperName());
    return C_SlotManager__ConnectToQuota_original(pThis, slotWrapper, name, slotType, unk);
}

static InitFunction init([]() {
    const auto C_SlotManager__ConnectToQuota_Addr = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 70 FF 81 ? ? ? ?").get_first());
    MH_CreateHook((LPVOID)C_SlotManager__ConnectToQuota_Addr, (PBYTE)C_SlotManager__ConnectToQuota, reinterpret_cast<void **>(&C_SlotManager__ConnectToQuota_original));
});
