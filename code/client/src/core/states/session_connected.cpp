#include "session_connected.h"
#include "states.h"

#include <utils/states/machine.h>

#include <external/imgui/widgets/corner_text.h>

#include "core/application.h"

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
        // Open Chat without disabling controls
        gApplication->GetChat()->Open(false);

        return true;
    }

    bool SessionConnectedState::OnExit(Framework::Utils::States::Machine *) {
        gApplication->GetChat()->Close();

        return true;
    }

    bool SessionConnectedState::OnUpdate(Framework::Utils::States::Machine *) {
        gApplication->GetImGUI()->PushWidget([]() {
            gApplication->GetChat()->Update();

            using namespace Framework::External::ImGUI::Widgets; // For DrawCornerText() and Corner enum
            DrawCornerText(CORNER_RIGHT_TOP, "YOU ARE CONNECTED");
            DrawCornerText(CORNER_RIGHT_TOP, "Press F9 to disconnect");
        });

        if (gApplication->GetInput()->IsKeyPressed(FW_KEY_F9)) {
            gApplication->GetNetworkingEngine()->GetNetworkClient()->Disconnect();
            return true;
        }
        return false;
    }
} // namespace MafiaMP::Core::States
