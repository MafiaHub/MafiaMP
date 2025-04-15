#pragma once

#include "../ui_base.h"

#include <utils/time.h>

namespace MafiaMP::Core::UI::Devs {
    class NetworkStats final: public UIBase {
      private:
        int64_t _nextStatsUpdate = 0;

        char _stats[8192] = {0};

        virtual void OnOpen() override;

        virtual void OnClose() override;

        virtual void OnUpdate() override;

      public:
        NetworkStats(): UIBase() {};
    };
} // namespace MafiaMP::Core::UI::Devs
