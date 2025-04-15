#include "session_connection.h"
#include "states.h"

#include "../application.h"

#include <logging/logger.h>

#include <external/imgui/widgets/corner_text.h>

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
        gApplication->LockControls(true);

        if (!MafiaMP::Core::gApplication->GetNetworkingEngine()->Connect(appState._host, appState._port, "")) {
            Framework::Logging::GetInstance()->Get("SessionConnectionState")->error("Connection to server failed");
            machine->RequestNextState(StateIds::MainMenu);
            return true;
        }
        return true;
    }

    bool SessionConnectionState::OnExit(Framework::Utils::States::Machine *) {
        gApplication->LockControls(false);
        return true;
    }

    bool SessionConnectionState::OnUpdate(Framework::Utils::States::Machine *) {
        gApplication->GetImGUI()->PushWidget([&]() {
            Framework::External::ImGUI::Widgets::DrawCornerText(Framework::External::ImGUI::Widgets::CORNER_RIGHT_TOP, "CONNECTING...");
        });
        return false;
    }
} // namespace MafiaMP::Core::States
