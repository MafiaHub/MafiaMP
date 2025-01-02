#pragma once

#include <cstdint>
#include "sdk/entities/human/c_human_inventory.h"

namespace SDK {
    class C_InventoryWrapper {
      public:
        void AddMoney(uint64_t);
        void AddWeapon(int, int);

        uint64_t TellMoney();

        C_HumanInventory *GetAsHumanInventory() {
            return reinterpret_cast<C_HumanInventory*>(this);
        }
    };
}
