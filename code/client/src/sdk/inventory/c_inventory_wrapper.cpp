#include "c_inventory_wrapper.h"

#include <utils/hooking/hooking.h>
#include "../patterns.h"

namespace SDK {
    void C_InventoryWrapper::AddMoney(uint64_t money) {
        hook::this_call(gPatterns.C_InventoryWrapper__AddMoney, this, money);
    }

    void C_InventoryWrapper::AddWeapon(int weapon, int ammo) {
        hook::this_call(gPatterns.C_InventoryWrapper__AddWeapon, this, weapon, ammo);
    }

    uint64_t C_InventoryWrapper::TellMoney() {
        return hook::this_call<uint64_t>(gPatterns.C_InventoryWrapper__TellMoney, this);
    }
}
