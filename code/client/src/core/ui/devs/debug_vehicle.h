#pragma once

#include "../ui_base.h"

namespace MafiaMP::Core::UI::Devs {
    class DebugVehicle final: public UIBase {
      private:
        virtual void OnOpen() override;

        virtual void OnClose() override;

        virtual void OnUpdate() override;

      public:
        DebugVehicle(): UIBase() {};
    };
} // namespace MafiaMP::Core::UI::Devs
