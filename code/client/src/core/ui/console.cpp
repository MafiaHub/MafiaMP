#include "console.h"

#include <utils/safe_win32.h>

namespace MafiaMP::Core::UI {
    Console::Console(std::shared_ptr<Framework::Utils::CommandProcessor> commandProcessor): Framework::External::ImGUI::Widgets::Console(commandProcessor), Core::UI::UIBase() {}
} // namespace MafiaMP::Core::UI
