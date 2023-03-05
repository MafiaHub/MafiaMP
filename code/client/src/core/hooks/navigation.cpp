#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

class C_DistrictDefinition {

};

typedef bool(__fastcall *S_ScriptDistrictChangeListener__OnDistrictChange_t)(void *, C_DistrictDefinition const &);
S_ScriptDistrictChangeListener__OnDistrictChange_t S_ScriptDistrictChangeListener__OnDistrictChange_original = nullptr;

bool S_ScriptDistrictChangeListener__OnDistrictChange(void *pThis, C_DistrictDefinition const &def) {
    Framework::Logging::GetLogger("Hooks")->debug("OnDistrictChange {}", fmt::ptr(&def));
    return S_ScriptDistrictChangeListener__OnDistrictChange_original(pThis, def);
}

static InitFunction init([]() {
    const auto S_ScriptDistrictChangeListener__OnDistrictChange_Addr = reinterpret_cast<uint64_t>(hook::pattern("48 83 EC 68 0F 10 02").get_first());
    MH_CreateHook((LPVOID)S_ScriptDistrictChangeListener__OnDistrictChange_Addr, (PBYTE)S_ScriptDistrictChangeListener__OnDistrictChange, reinterpret_cast<void **>(&S_ScriptDistrictChangeListener__OnDistrictChange_original));
});
