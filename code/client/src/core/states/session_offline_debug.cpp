#include <utils/safe_win32.h>
#include "session_offline_debug.h"
#include "states.h"

#include "../../game/helpers/controls.h"

#include <utils/states/machine.h>

#include <external/imgui/widgets/corner_text.h>

#include "../application.h"

namespace MafiaMP::Core::States {
    SessionOfflineDebugState::SessionOfflineDebugState() {}

    SessionOfflineDebugState::~SessionOfflineDebugState() {}

    int32_t SessionOfflineDebugState::GetId() const {
        return StateIds::SessionOfflineDebug;
    }

    const char *SessionOfflineDebugState::GetName() const {
        return "SessionOfflineDebug";
    }

    bool SessionOfflineDebugState::OnEnter(Framework::Utils::States::Machine *) {
        // Reset camera by player
        //TODO

        // Give back controls
        MafiaMP::Game::Helpers::Controls::Lock(false);
        return true;
    }

    bool SessionOfflineDebugState::OnExit(Framework::Utils::States::Machine *) {
        return true;
    }

    bool SessionOfflineDebugState::OnUpdate(Framework::Utils::States::Machine *machine) {
        bool shouldProceed = false;

        if (GetAsyncKeyState(VK_F9) & 0x1) {
            machine->RequestNextState(StateIds::Menu);
            shouldProceed = true;
        }

        gApplication->GetImGUI()->PushWidget([&]() {
            using namespace Framework::External::ImGUI::Widgets;
            DrawCornerText(CORNER_RIGHT_TOP, "OFFLINE DEBUG MODE");
            DrawCornerText(CORNER_RIGHT_TOP, "Press F9 to return to menu");
        });

        return shouldProceed;
    }
} // namespace MafiaMP::Core::States
