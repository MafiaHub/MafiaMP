#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include "sdk/ue/sys/math/c_vector.h"
#include "sdk/ue/game/translocator/c_translocator.h"

typedef void(__fastcall *C_Translocator__SpawnObject_t)(void *, SDK::ue::game::translocator::C_Translocator::S_ObjectToSpawn const &);
C_Translocator__SpawnObject_t C_Translocator__SpawnObject_original = nullptr;

void C_Translocator__SpawnObject(void *pThis, SDK::ue::game::translocator::C_Translocator::S_ObjectToSpawn &obj) {
    Framework::Logging::GetLogger("Hooks")->debug("Spawning object: pObject={}, index={}, pos=({}, {}, {})", (void *)obj.pObject, obj.index, obj.pos.x, obj.pos.y, obj.pos.z);
    C_Translocator__SpawnObject_original(pThis, obj);
}

static InitFunction init([]() {
    // const auto C_Translocator__SpawnObject_Addr = hook::get_opcode_address("E8 ? ? ? ? 48 83 43 ? ? 48 8B 43 50");
    // MH_CreateHook((LPVOID)C_Translocator__SpawnObject_Addr, (PBYTE)C_Translocator__SpawnObject, reinterpret_cast<void **>(&C_Translocator__SpawnObject_original));
    },
    "Translocator");
