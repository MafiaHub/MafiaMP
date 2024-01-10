#pragma once

#include <stdint.h>

namespace SDK {
    namespace mafia::tables {
        enum class E_RadioStation : uint32_t {
            E_RADIO_1,
            E_RADIO_2,
            E_RADIO_3,
            E_RADIO_POLICE,
            E_RADIO_RANDOM,
            E_RADIO_LAST,
        }
    } // namespace mafia::tables::vehicle
} // namespace SDK
