#pragma once

#include <set>
#include <string>

namespace MafiaMP::Scripting {

    /**
     * MafiaMP-specific reserved event names that cannot be emitted by user code.
     * These extend the Framework's reserved events.
     */
    inline const std::set<std::string> RESERVED_EVENTS = {
        "playerDied",
        "vehiclePlayerEnter",
        "vehiclePlayerLeave",
        "chatMessage",
        "chatCommand"
    };

} // namespace MafiaMP::Scripting
