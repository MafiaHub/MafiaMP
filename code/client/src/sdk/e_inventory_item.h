#pragma once

#include <cstdint>

namespace SDK {
    enum class E_InventoryItem : int32_t {
        E_ITEM_UNKNOWN     = -1,
        E_ITEM_WEAPON      = 0,
        E_ITEM_SCRIPT      = 2,
        E_ITEM_INVENTORY   = 8,
        E_ITEM_LIFT_BUTTON = 0xB,
        E_ITEM_UPGRADE     = 0xC,
        E_ITEM_MEDKIT      = 0xD,
        E_ITEM_END         = 0x11
    };
}
