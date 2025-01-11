#include "c_human_inventory.h"

#include "../../patterns.h"

namespace SDK {
    void C_HumanInventory::AddMedkits(int amount) {
        hook::this_call(gPatterns.C_HumanInventory__AddMedkits, this, amount);
    }

    void C_HumanInventory::AddMoney(long long amnt) {
        hook::this_call(gPatterns.C_HumanInventory__AddMoney, this, amnt);
    }

    bool C_HumanInventory::CanFire() {
        return hook::this_call<bool>(gPatterns.C_HumanInventory__CanFire, this);
    }

    bool C_HumanInventory::CanReload() {
        return hook::this_call<bool>(gPatterns.C_HumanInventory__CanReload, this);
    }

    bool C_HumanInventory::CanUseMedkit() {
        return hook::this_call<bool>(gPatterns.C_HumanInventory__CanUseMedkit, this);
    }

    bool C_HumanInventory::DoReload(int weapon, int ammo) {
        return hook::this_call<bool>(gPatterns.C_HumanInventory__DoReload, this, weapon, ammo);
    }

    bool C_HumanInventory::DoShot() {
        return hook::this_call<bool>(gPatterns.C_HumanInventory__DoShot, this);
    }

    void C_HumanInventory::RemoveMedkits(int amount) {
        // TODO: implement
    }

    int64_t C_HumanInventory::TellMedkit() {
        return hook::this_call<int64_t>(gPatterns.C_HumanInventory__TellMedkit, this);
    }

    void C_HumanInventory::UseMedkit() {
        hook::this_call(gPatterns.C_HumanInventory__UseMedkit, this);
    }
} // namespace SDK
