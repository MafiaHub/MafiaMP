#include "session_connected.h"
#include "states.h"

namespace MafiaMP::Core::States {
    SessionConnectedState::SessionConnectedState() {}

    SessionConnectedState::~SessionConnectedState() {}

    int32_t SessionConnectedState::GetId() const {
        return StateIds::SessionConnected;
    }

    const char *SessionConnectedState::GetName() const {
        return "SessionConnected";
    }

    bool SessionConnectedState::OnEnter() {
        return true;
    }

    bool SessionConnectedState::OnExit() {
        return true;
    }

    bool SessionConnectedState::OnUpdate() {
        return true;
    }
} // namespace MafiaMP::Core::States
