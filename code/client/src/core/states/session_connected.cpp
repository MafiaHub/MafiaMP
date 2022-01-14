#include "session_connected.h"
#include "states.h"

#include <utils/states/machine.h>

namespace MafiaMP::Core::States {
    SessionConnectedState::SessionConnectedState() {}

    SessionConnectedState::~SessionConnectedState() {}

    int32_t SessionConnectedState::GetId() const {
        return StateIds::SessionConnected;
    }

    const char *SessionConnectedState::GetName() const {
        return "SessionConnected";
    }

    bool SessionConnectedState::OnEnter(Framework::Utils::States::Machine *) {
        return true;
    }

    bool SessionConnectedState::OnExit(Framework::Utils::States::Machine *) {
        return true;
    }

    bool SessionConnectedState::OnUpdate(Framework::Utils::States::Machine *) {
        return true;
    }
} // namespace MafiaMP::Core::States
