#include "session_connection.h"
#include "states.h"

namespace MafiaMP::Core::States {
    SessionConnectionState::SessionConnectionState() {}

    SessionConnectionState::~SessionConnectionState() {}

    int32_t SessionConnectionState::GetId() const {
        return StateIds::SessionConnection;
    }

    const char *SessionConnectionState::GetName() const {
        return "SessionConnection";
    }

    bool SessionConnectionState::OnEnter() {
        return true;
    }

    bool SessionConnectionState::OnExit() {
        return true;
    }

    bool SessionConnectionState::OnUpdate() {
        return true;
    }
} // namespace MafiaMP::Core::States
