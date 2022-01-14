#include "session_connection.h"
#include "states.h"

#include <utils/states/machine.h>

namespace MafiaMP::Core::States {
    SessionConnectionState::SessionConnectionState() {}

    SessionConnectionState::~SessionConnectionState() {}

    int32_t SessionConnectionState::GetId() const {
        return StateIds::SessionConnection;
    }

    const char *SessionConnectionState::GetName() const {
        return "SessionConnection";
    }

    bool SessionConnectionState::OnEnter(Framework::Utils::States::Machine *) {
        return true;
    }

    bool SessionConnectionState::OnExit(Framework::Utils::States::Machine *) {
        return true;
    }

    bool SessionConnectionState::OnUpdate(Framework::Utils::States::Machine *) {
        return true;
    }
} // namespace MafiaMP::Core::States
