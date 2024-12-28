#pragma once

#include "../ui_base.h"

namespace MafiaMP::Core::UI::Devs {
    class DebugPlayer final: public UIBase {
      private:
        virtual void OnOpen() override;

        virtual void OnClose() override;

        virtual void OnUpdate() override;

      public:
        DebugPlayer(): UIBase() {};
    };
} // namespace MafiaMP::Core::UI::Devs
