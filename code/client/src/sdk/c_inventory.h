#pragma once

#include <cstdint>
#include <vector>
#include "sdk/c_inventory_item.h"
#include "sdk/ue/game/e_inventory_slot.h"
#include "sdk/ue/sys/utils/c_hash_name.h"

namespace SDK {
    class I_Inventory {
      public:
        virtual ~I_Inventory() {};
    };

    struct AmmoEntry {
        uint32_t categoryID;
        uint32_t count;
    };

    class C_Inventory: public I_Inventory {
      public:
        int32_t m_someIndex = -1;                               // 0008 - 000C
        int32_t unk1;                                           // 000C - 0010
        std::vector<AmmoEntry> m_ammoStorage;                   // 0010 - 0028
        std::vector<ue::sys::utils::C_HashName> m_specialItems; // 0028 - 0040
        std::vector<C_InventoryItem> m_items;                   // 0040 - 0058
        ue::game::E_InventorySlot m_slots[2];                   // 0058 - 0060
        int32_t m_finalFlag = 0;                                // 0060 - 0064
        int32_t unk2;                                           // 0064 - 0068
    };

    static_assert(sizeof(C_Inventory) == 0x68, "Wrong size: C_Inventory");
}
