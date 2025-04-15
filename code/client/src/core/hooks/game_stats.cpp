#include <utils/hooking/hooking.h>
#include <utils/hooking/hook_function.h>

static InitFunction init([]() {
    const auto C_GameStatsModule__AddPlayerBulletShots_Addr = reinterpret_cast<uint64_t>(hook::pattern("80 79 24 00 74 12").get_first());
    hook::return_function(C_GameStatsModule__AddPlayerBulletShots_Addr);

    const auto C_GameStatsModule__AddPlayerDrunk_Addr = reinterpret_cast<uint64_t>(hook::pattern("80 79 24 00 74 11 8B 51 20 48 8B 41 08 48 8B 0C D0 FF 81 ? ? ? ? C3 CC").get_first());
    hook::return_function(C_GameStatsModule__AddPlayerDrunk_Addr);

    const auto C_GameStatsModule__AddPlayerWalkedDistance_Addr = reinterpret_cast<uint64_t>(hook::pattern("80 79 24 00 74 15").get_first());
    hook::return_function(C_GameStatsModule__AddPlayerWalkedDistance_Addr);

    const auto C_GameStatsModule__AddStatisticFromScript_Addr = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 57 48 83 EC 20 80 79 24 00").get_first());
    hook::return_function(C_GameStatsModule__AddStatisticFromScript_Addr);

    const auto C_GameStatsModule__Damage_Addr = hook::get_opcode_address("E8 ? ? ? ? 8B 9D ? ? ? ? 44 8B 75 BC");
    hook::return_function(C_GameStatsModule__Damage_Addr);

    const auto C_GameStatsModule__PlayerUseCar_Addr = hook::get_opcode_address("E8 ? ? ? ? 48 8B 87 ? ? ? ? 8B 48 08");
    hook::return_function(C_GameStatsModule__PlayerUseCar_Addr);

    const auto C_GameStatsModule__PlayerUseWeapon_Addr = reinterpret_cast<uint64_t>(hook::pattern("48 83 EC 28 83 C8 FF 45 84 C0").get_first());
    hook::return_function(C_GameStatsModule__PlayerUseWeapon_Addr);

    const auto C_GameStatsModule__SetProgress_Addr = reinterpret_cast<uint64_t>(hook::pattern("80 79 24 00 4C 8B C9").get_first());
    hook::return_function(C_GameStatsModule__SetProgress_Addr);
    },
    "GameStats");
