#include <MinHook.h>
#include <utils/hooking/hooking.h>
#include <utils/hooking/hook_function.h>

#include <logging/logger.h>

typedef void(__fastcall *C_GameDirector__OnActiveCityChanged_t)(void *_this, char const *);
C_GameDirector__OnActiveCityChanged_t C_GameDirector__OnActiveCityChanged_original = nullptr;
void __fastcall C_GameDirector__OnActiveCityChanged(void *_this, char const *activeCity) {
    Framework::Logging::GetLogger("Hooks")->debug("Active city changed, it is now: {}", activeCity);
    return C_GameDirector__OnActiveCityChanged_original(_this, activeCity);
}


static InitFunction init([]() {
    const auto C_GameDirector__OnActiveCityChanged_Addr = hook::get_opcode_address("E8 ? ? ? ? 80 BE ? ? ? ? ? 0F 84 ? ? ? ? 48 89 5C 24 ?");
    MH_CreateHook((LPVOID)C_GameDirector__OnActiveCityChanged_Addr, (PBYTE)C_GameDirector__OnActiveCityChanged, reinterpret_cast<void **>(&C_GameDirector__OnActiveCityChanged_original));
});
