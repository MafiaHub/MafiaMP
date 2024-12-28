#pragma once

#include <utils/safe_win32.h>

#include "ui_base.h"

#include <external/imgui/widgets/console.h>
#include <utils/command_processor.h>

namespace MafiaMP::Core::UI {
    class Console
        : public Framework::External::ImGUI::Widgets::Console
        , public Core::UI::UIBase {
      public:
        Console(std::shared_ptr<Framework::Utils::CommandProcessor> commandProcessor);
        ~Console() = default;
    };
} // namespace MafiaMP::Core::UI
