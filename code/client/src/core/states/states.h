#pragma once

namespace MafiaMP::Core::States {
    enum StateIds: int32_t {
        Initialize,
        MainMenu,
        SessionConnection,
        SessionConnected,
        SessionDisconnection,
        SessionOfflineDebug,
        Shutdown
    };
}
