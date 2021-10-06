#pragma once

#include <cstdint>

namespace SDK {
    class C_HumanInventory {
      public:
        char pad0[0xA0];                      // 0000 - 00A0
        uint64_t m_iMoney;                    // 00A0 - 00A8
        char pad1[0x48];                      // 00A8 - 00F0
        bool m_bUnlimitedAmmo;                // 00F0 - 00F1
        bool m_bUnlimitedAmmoInWeakestWeapon; // 00F1 - 00F2

      public:
        void AddMedkits(int);

        bool CanFire();
        bool CanReload();

        bool CanUseMedkit();

        bool DoReload(int, int);
        bool DoShot();

        void RemoveMedkits(int);

        int64_t TellMedkit();

        void UseMedkit();
    };
}
