#include "c_time_budget_info.h"

#include "patterns.h"

namespace SDK {
    void C_TimeBudgetInfo::C_Ctx::BeginUpdate(C_TimeBudgetInfo *pTimeBudgetInfo) {
        hook::this_call<void>(gPatterns.C_Ctx__BeginUpdate, this, pTimeBudgetInfo);
    }

    void C_TimeBudgetInfo::C_Ctx::EndUpdate() {
        hook::this_call<void>(gPatterns.C_Ctx__EndUpdate, this);
    }
} // namespace SDK
