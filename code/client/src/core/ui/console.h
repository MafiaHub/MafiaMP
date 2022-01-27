#pragma once

#include <utils/safe_win32.h>

#include <external/imgui/widgets/console.h>
#include <integrations/client/instance.h>
#include <utils/command_processor.h>
#include <utils/states/machine.h>
#include <utils/states/state.h>

#include <cxxopts.hpp>
#include <functional>
#include <memory>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <vector>

namespace MafiaMP::Core::UI {
    class MafiaConsole: public Framework::External::ImGUI::Widgets::Console {
      public:
        MafiaConsole(std::shared_ptr<Framework::Utils::CommandProcessor> commandProcessor);
        ~MafiaConsole() = default;

        bool Open();
        bool Close();

        virtual void LockControls(bool lock) override;
    };
} // namespace MafiaMP::Core::UI
