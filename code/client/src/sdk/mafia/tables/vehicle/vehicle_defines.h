#pragma once

#include <stdint.h>

namespace SDK {
    namespace mafia::tables::vehicle {
        enum class E_VehiclesTableFunctionFlags : uint32_t {
            E_VTFF_MEDIUM_SIZE                = 0x1,
            E_VTFF_BIG_CAR                    = 0x2,
            E_VTFF_ARMORED                    = 0x4,
            E_VTFF_OPEN_TOP                   = 0x8,
            E_VTFF_DISABLE_TRUNK              = 0x10,
            E_VTFF_SOFT_TOP                   = 0x20,
            E_VTFF_DOUBLE_COLOR               = 0x40,
            E_VTFF_NO_GARAGE                  = 0x80,
            E_VTFF_NO_BODYSHOP                = 0x100,
            E_VTFF_NO_REPAINT                 = 0x200,
            E_VTFF_NO_AIRBRUSH                = 0x400,
            E_VTFF_NO_PLATE                   = 0x800,
            E_VTFF_NO_WHEEL_CHANGE            = 0x1000,
            E_VTFF_NO_WINDOW_TINT             = 0x2000,
            E_VTFF_DURABLE_WHEELS             = 0x4000,
            E_VTFF_DLC                        = 0x8000,
            E_VTFF_SHOW_IN_CARCYCLOPEDIA      = 0x10000,
            E_VTFF_PERSISTENT_COVER_COLLISION = 0x20000,
            E_VTFF_BONUS                      = 0x40000,
            E_VTFF_NO_FLIPOVER                = 0x80000,
            E_VTFF_UNDRIVEABLE                = 0x100000,
            E_VTFF_VISIBLE_CARGO_SPACE        = 0x200000,
            E_VTFF_SINGLE_SEATER              = 0x400000,
        };

        enum class E_VehicleRaceClass : uint32_t { 
            E_VRC_NONE, 
            E_VRC_EXOTIC,
            E_VRC_SPORT,
            E_VRC_STREET,
            E_VRC_STANDARD,
            E_VRC_UTILITY 
        };

    } // namespace mafia::tables::vehicle
} // namespace SDK
