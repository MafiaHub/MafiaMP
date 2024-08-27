#include "session_disconnection.h"
#include "states.h"

#include <utils/states/machine.h>

#include "core/application.h"

namespace MafiaMP::Core::States {
    SessionDisconnectionState::SessionDisconnectionState() {}

    SessionDisconnectionState::~SessionDisconnectionState() {}

    int32_t SessionDisconnectionState::GetId() const {
        return StateIds::SessionDisconnection;
    }

    const char *SessionDisconnectionState::GetName() const {
        return "SessionDisconnection";
    }

    bool SessionDisconnectionState::OnEnter(Framework::Utils::States::Machine *machine) {
        machine->RequestNextState(StateIds::MainMenu);

        return true;
    }

    bool SessionDisconnectionState::OnExit(Framework::Utils::States::Machine *) {
        return true;
    }

    bool SessionDisconnectionState::OnUpdate(Framework::Utils::States::Machine *) {
        return true;
    }
} // namespace MafiaMP::Core::States
