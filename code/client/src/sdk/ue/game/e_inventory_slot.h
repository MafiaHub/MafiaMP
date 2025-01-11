#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::game {
        enum class E_InventorySlot : int32_t {
            E_IST_SMALL_WEP         = 0,  // Small weapons
            E_IST_BIG_WEP           = 1,  // Big weapons
            E_IST_EXPLOSIVES        = 2,  // Explosives
            E_IST_KNIVES            = 3,  // Knives
            E_IST_MELEE_WEP         = 4,  // Melee weapons
            E_IST_PLACED_EXPLOSIVES = 6,  // Placed explosives
            E_IST_GRENADES          = 7,  // Grenades
            E_IST_MOLOTOVS          = 8,  // Molotov cocktails
            E_IST_VOODOO            = 9,  // Voodoo items
            E_IST_PROXIMITY_MINES   = 10, // Proximity mines
            E_IST_SNIPER_SERVICE    = 11, // Sniper service
            E_IST_THROWING_KNIFE    = 12, // Throwing knives
            E_IST_COIN              = 13, // Coins
            E_IST_INVALID           = 14  // Invalid slot
        };
    }
}
