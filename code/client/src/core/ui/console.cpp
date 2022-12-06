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
    MafiaConsole::MafiaConsole(std::shared_ptr<Framework::Utils::CommandProcessor> commandProcessor, std::shared_ptr<Framework::Input::IInput> input): Framework::External::ImGUI::Widgets::Console(commandProcessor, input) {}

    void MafiaConsole::LockControls(bool lock) {
        gApplication->LockControls(lock);
    }
} // namespace MafiaMP::Core::UI
