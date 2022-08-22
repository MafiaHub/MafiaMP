#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

typedef bool(__fastcall *C_HumanWeaponController__SetAiming_t)(void *, bool);
C_HumanWeaponController__SetAiming_t C_HumanWeaponController__SetAiming_Original = nullptr;
bool C_HumanWeaponController__SetAiming(void* _this, bool aiming) {
    return C_HumanWeaponController__SetAiming_Original(_this, aiming);
}

typedef bool(__fastcall *C_HumanWeaponController__SetFirePressedFlag_t)(void *, bool);
C_HumanWeaponController__SetFirePressedFlag_t C_HumanWeaponController__SetFirePressedFlag_Original = nullptr;
bool C_HumanWeaponController__SetFirePressedFlag(void *_this, bool firePressed) {
    return C_HumanWeaponController__SetFirePressedFlag_Original(_this, firePressed);
}


static InitFunction init([]() {
    const auto addr1 = hook::get_opcode_address("E8 ? ? ? ? 48 8B 43 10 48 8B 88 ? ? ? ? C6 41 14 00");
    MH_CreateHook((LPVOID)addr1, (PBYTE)C_HumanWeaponController__SetAiming, reinterpret_cast<void **>(&C_HumanWeaponController__SetAiming_Original));

    const auto addr2 = hook::pattern("84 D2 75 11 33 C0").get_first();
    MH_CreateHook((LPVOID)addr2, (PBYTE)C_HumanWeaponController__SetFirePressedFlag, reinterpret_cast<void **>(&C_HumanWeaponController__SetFirePressedFlag_Original));
});
