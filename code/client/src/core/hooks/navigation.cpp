#include <utils/safe_win32.h>

#include <MinHook.h>

#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include "../application.h"

#include "../../game/helpers/districts.h"

#include "sdk/mafia/framework/director/c_game_director.h"

typedef int64_t(__fastcall *C_Navigation__OnDistrictChange_t)(void *, SDK::mafia::framework::director::I_GameDirector::C_DistrictDefinition const &);
C_Navigation__OnDistrictChange_t C_Navigation__OnDistrictChange_original = nullptr;

int64_t C_Navigation__OnDistrictChange(void *pThis, SDK::mafia::framework::director::I_GameDirector::C_DistrictDefinition const &def) {
    if (MafiaMP::Core::gApplication && MafiaMP::Core::gApplication->IsInitialized()) {
        const auto newDistrictID = static_cast<MafiaMP::Game::Helpers::Districts>(def.districtID);
        Framework::Logging::GetLogger("Hooks")->trace("C_Navigation::OnDistrictChange: New district is {}", newDistrictID);
        MafiaMP::Core::gApplication->SetLastDistrictID(newDistrictID);
    }
    return C_Navigation__OnDistrictChange_original(pThis, def);
}

static InitFunction init([]() {
    const auto C_Navigation__OnDistrictChange_Addr = hook::get_pattern<uint64_t>("40 53 57 48 83 EC 28 8B 02");
    MH_CreateHook((LPVOID)C_Navigation__OnDistrictChange_Addr, (PBYTE)C_Navigation__OnDistrictChange, reinterpret_cast<void **>(&C_Navigation__OnDistrictChange_original));
});
