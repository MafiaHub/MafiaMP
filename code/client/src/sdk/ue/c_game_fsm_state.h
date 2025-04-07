#pragma once

#include <sdk/c_time_budget_info.h>

namespace SDK {
    class C_GameFSMState {
      public:
          C_TimeBudgetInfo *m_pTimeBudgetInfo; // 0008 - 0010

      public:
        virtual ~C_GameFSMState() = default;

    };
}
