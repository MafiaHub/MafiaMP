#include <utils/safe_win32.h>

#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include "core/modules/human.h"

#include "sdk/ue/game/human/c_agent.h"

// A ped's agent copies the root rotation of its animation into its physics controller every
// update, which is how a turn animation turns the ped. A remote ped plays turns of its own, not
// its owner's: it is held on the replicated rotation instead.
namespace {
    using C_Agent__ApplyAnimRotation_t                             = void(__fastcall *)(SDK::ue::game::human::C_Agent *);
    C_Agent__ApplyAnimRotation_t C_Agent__ApplyAnimRotation_original = nullptr;

    void __fastcall C_Agent__ApplyAnimRotation(SDK::ue::game::human::C_Agent *agent) {
        if (auto *human = MafiaMP::Core::Modules::Human::GetByAgent(agent); human && human->HoldsRotation()) {
            return;
        }
        C_Agent__ApplyAnimRotation_original(agent);
    }
} // namespace

static InitFunction init(
    []() {
        const auto addr = hook::get_pattern("48 83 EC 38 48 8B 81 D8 01 00 00");
        MH_CreateHook(addr, reinterpret_cast<LPVOID>(C_Agent__ApplyAnimRotation), reinterpret_cast<void **>(&C_Agent__ApplyAnimRotation_original));
    },
    "C_Agent");
