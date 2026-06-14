#pragma once

#include "../ui_base.h"

#include <string>

namespace MafiaMP::Core::UI::Devs {
    class DebugPrefab final: public UIBase {
      private:
        int _selectedIndex   = -1;
        int _typeFilter      = -1;
        char _guidFilter[32] = "";

        virtual void OnOpen() override;

        virtual void OnClose() override;

        virtual void OnUpdate() override;

      public:
        DebugPrefab(): UIBase() {};
    };
} // namespace MafiaMP::Core::UI::Devs
