#include "console.h"
#include "../application.h"
#include <cxxopts/cxxopts.hpp>
#include <external/optick/wrapper.h>
#include <fmt/core.h>
#include <imgui/imgui.h>
#include <logging/logger.h>
#include <numeric>
#include <regex>
#include <sstream>
#include <utils/safe_win32.h>

#include "../../game/helpers/controls.h"

namespace MafiaMP::Core::UI {
    MafiaConsole::MafiaConsole(std::shared_ptr<Framework::Utils::CommandProcessor> commandProcessor): Framework::External::ImGUI::Widgets::Console(commandProcessor) {}

    void MafiaConsole::LockControls(bool lock) {
        if (lock) {
            // Lock game controls
            Game::Helpers::Controls::Lock(true);

            // Enable cursor
            Core::gApplication->GetImGUI()->ShowCursor(true);
        }
        else {
            // Unlock game controls
            Game::Helpers::Controls::Lock(false);

            // Disable cursor
            Core::gApplication->GetImGUI()->ShowCursor(false);
        }
    }
} // namespace MafiaMP::Core::UI
