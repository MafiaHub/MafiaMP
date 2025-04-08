#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include "sdk/ue/sys/utils/c_hash_name.h"
#include "sdk/ue/c_string.h"
#include "sdk/ue/sys/core/c_core.h"
#include "sdk/ue/sys/core/c_resource_list.h"

class I_InputStream {};

class S_LoadParams {};

typedef int64_t(__fastcall *C_ResourceList__LoadConfigXml_t)(void *);
C_ResourceList__LoadConfigXml_t C_ResourceList__LoadConfigXml_original = nullptr;

int64_t C_ResourceList__LoadConfigXml(void *pThis) {
    Framework::Logging::GetLogger("Hooks")->debug("Loading Config XML");
    return C_ResourceList__LoadConfigXml_original(pThis);
}

typedef int64_t(__fastcall *C_ResourceList__LoadResource_t)(void *, SDK::ue::sys::utils::C_HashName const &, I_InputStream &, unsigned int, S_LoadParams const &, char const *);
C_ResourceList__LoadResource_t C_ResourceList__LoadResource_original = nullptr;

int64_t C_ResourceList__LoadResource(void *pThis, SDK::ue::sys::utils::C_HashName const &name, I_InputStream &stream, unsigned int a3, S_LoadParams const &params, char const *a5) {
    Framework::Logging::GetLogger("Hooks")->debug("Loading Resource: {} = {} ({})", name.GetValue(), a5, a3);
    return C_ResourceList__LoadResource_original(pThis, name, stream, a3, params, a5);
}

typedef bool(__fastcall *C_ResourceList__IsResourceLoaded_t)(void *, SDK::ue::C_String const &);
C_ResourceList__IsResourceLoaded_t C_ResourceList__IsResourceLoaded_original = nullptr;

bool C_ResourceList__IsResourceLoaded(void *pThis, SDK::ue::C_String const &name) {
    Framework::Logging::GetLogger("Hooks")->debug("Is Resource Loaded");
    return C_ResourceList__IsResourceLoaded_original(pThis, name);
}

static InitFunction init([]() {
    // const auto C_ResourceList__LoadConfigXml_Addr = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 81 EC ? ? ? ? 48 8B D9 C6 44 24 ? ? 48 8D 4C 24").get_first());
    // MH_CreateHook((LPVOID)C_ResourceList__LoadConfigXml_Addr, (PBYTE)C_ResourceList__LoadConfigXml, reinterpret_cast<void **>(&C_ResourceList__LoadConfigXml_original));

    // const auto C_ResourceList__LoadResource_Addr = reinterpret_cast<uint64_t>(hook::pattern("4C 89 44 24 ? 48 89 54 24 ? 55 57 41 54 41 56").get_first());
    // MH_CreateHook((LPVOID)C_ResourceList__LoadResource_Addr, (PBYTE)C_ResourceList__LoadResource, reinterpret_cast<void **>(&C_ResourceList__LoadResource_original));

    // const auto C_ResourceList__IsResourceLoaded_Addr = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B 12 48 8B F9 48 85 D2 48 8D 4C 24").get_first());
    // MH_CreateHook((LPVOID)C_ResourceList__IsResourceLoaded_Addr, (PBYTE)C_ResourceList__IsResourceLoaded, reinterpret_cast<void **>(&C_ResourceList__IsResourceLoaded_original));
    },
    "ResourceList");
