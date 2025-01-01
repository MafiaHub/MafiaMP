#pragma once

#include <cstdint>

#include "sdk/c_inventory.h"

namespace SDK {
    class C_HumanInventory : public C_Inventory {
      public:
        // Base class C_Inventory is 0x68
        char pad0[0x38];                      // 0068 - 00A0
        int64_t m_iMoney;                     // 00A0 - 00A8
        char pad1[0x48];                      // 00A8 - 00F0
        bool m_bUnlimitedAmmo;                // 00F0 - 00F1
        bool m_bUnlimitedAmmoInWeakestWeapon; // 00F1 - 00F2
        char pad2[0x6];                       // 00F2 - 00F8 (padding for alignment)

      public:
        void AddMedkits(int);
        void AddMoney(long long);

        bool CanFire();
        bool CanReload();

        bool CanUseMedkit();

        bool DoReload(int, int);
        bool DoShot();

        void RemoveMedkits(int);

        int64_t TellMedkit();

        void UseMedkit();
    };

    static_assert(sizeof(C_HumanInventory) == 0xF8, "Wrong size: C_HumanInventory");
}
