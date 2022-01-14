#include "session_disconnection.h"
#include "states.h"

namespace MafiaMP::Core::States {
    SessionDisconnectionState::SessionDisconnectionState() {}

    SessionDisconnectionState::~SessionDisconnectionState() {}

    int32_t SessionDisconnectionState::GetId() const {
        return StateIds::SessionDisconnection;
    }

    const char *SessionDisconnectionState::GetName() const {
        return "SessionDisconnection";
    }

    bool SessionDisconnectionState::OnEnter() {
        return true;
    }

    bool SessionDisconnectionState::OnExit() {
        return true;
    }

    bool SessionDisconnectionState::OnUpdate() {
        return true;
    }
} // namespace MafiaMP::Core::States
