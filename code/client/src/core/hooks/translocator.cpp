#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include "../../sdk/ue/sys/math/c_vector.h"

struct S_ObjectToSpawn {
    char pad0[0x10];
    SDK::ue::sys::math::C_Vector pos;
};

typedef void(__fastcall *C_Translocator__SpawnObject_t)(void *, S_ObjectToSpawn const &);
C_Translocator__SpawnObject_t C_Translocator__SpawnObject_original = nullptr;

void C_Translocator__SpawnObject(void* pThis, S_ObjectToSpawn & obj) {
    // Framework::Logging::GetLogger("Hooks")->debug("Spawning object {} = {}, {}, {}", fmt::ptr(&obj), obj.pos.x, obj.pos.y, obj.pos.z);
    C_Translocator__SpawnObject_original(pThis, obj);
}

static InitFunction init([]() {
    const auto C_Translocator__SpawnObject_Addr = hook::get_opcode_address("E8 ? ? ? ? 48 83 43 ? ? 48 8B 43 50");
    MH_CreateHook((LPVOID)C_Translocator__SpawnObject_Addr, (PBYTE)C_Translocator__SpawnObject, reinterpret_cast<void **>(&C_Translocator__SpawnObject_original));
});
