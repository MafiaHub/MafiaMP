#pragma once

namespace MafiaMP::Core::States {
    enum StateIds: int32_t {
        Initialize,
        Menu,
        SessionConnection,
        SessionConnected,
        SessionDisconnection,
        Shutdown
    };
}
