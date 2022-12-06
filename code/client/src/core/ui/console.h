#pragma once

#include <utils/safe_win32.h>

#include <external/imgui/widgets/console.h>
#include <utils/command_processor.h>

namespace MafiaMP::Core::UI {
    class MafiaConsole: public Framework::External::ImGUI::Widgets::Console {
      public:
        MafiaConsole(std::shared_ptr<Framework::Utils::CommandProcessor> commandProcessor, std::shared_ptr<Framework::Input::IInput> input);
        ~MafiaConsole() = default;

        virtual void LockControls(bool lock) override;
    };
} // namespace MafiaMP::Core::UI
