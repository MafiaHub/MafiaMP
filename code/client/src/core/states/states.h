#pragma once

namespace MafiaMP::Core::States {
    enum StateIds: int32_t {
        Initialize,
        GameReady,
        MainMenu,
        SessionConnection,
        SessionConnected,
        SessionDisconnection,
        SessionOfflineDebug,
        Shutdown
    };
}
