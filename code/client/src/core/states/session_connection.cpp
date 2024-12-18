#include "session_connection.h"
#include "states.h"

#include "../application.h"

#include <logging/logger.h>

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

    bool SessionConnectionState::OnEnter(Framework::Utils::States::Machine *machine) {
        const auto appState = MafiaMP::Core::gApplication->GetCurrentState();

        if (!MafiaMP::Core::gApplication->GetNetworkingEngine()->Connect(appState._host, appState._port, "")) {
            Framework::Logging::GetInstance()->Get("SessionConnectionState")->error("Connection to server failed");
            machine->RequestNextState(StateIds::MainMenu);
            return true;
        }
        return true;
    }

    bool SessionConnectionState::OnExit(Framework::Utils::States::Machine *) {
        return true;
    }

    bool SessionConnectionState::OnUpdate(Framework::Utils::States::Machine *) {
        return true;
    }
} // namespace MafiaMP::Core::States
