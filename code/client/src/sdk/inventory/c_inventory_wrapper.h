#pragma once

#include <cstdint>
#include "../entities/human/c_human_inventory.h"

namespace SDK {
    class C_InventoryWrapper {
      public:
        C_HumanInventory *m_pHumanInventory;

      public:
        void AddMoney(uint64_t);
        void AddWeapon(int, int);

        uint64_t TellMoney();

        C_HumanInventory *GetHumanInventory() {
            return m_pHumanInventory;
        }
    };
}
