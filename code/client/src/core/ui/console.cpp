#include <utils/safe_win32.h>

#include <cxxopts/cxxopts.hpp>
#include <fmt/core.h>
#include <imgui/imgui.h>
#include <numeric>
#include <regex>
#include <sstream>

#include <logging/logger.h>

#include "console.h"
#include "../application.h"

namespace MafiaMP::Core::UI {
    Console::Console(std::shared_ptr<Framework::Utils::CommandProcessor> commandProcessor): Framework::External::ImGUI::Widgets::Console(commandProcessor), Core::UI::UIBase() {}
} // namespace MafiaMP::Core::UI
