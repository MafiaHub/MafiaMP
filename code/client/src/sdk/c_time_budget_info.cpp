#include "c_time_budget_info.h"

#include "patterns.h"

namespace SDK {
    void C_TimeBudgetInfo::C_Ctx::BeginUpdate(C_TimeBudgetInfo *pTimeBudgetInfo) {
        hook::this_call<void>(gPatterns.C_Ctx__BeginUpdateAddr, this, pTimeBudgetInfo);
    }

    void C_TimeBudgetInfo::C_Ctx::EndUpdate() {
        hook::this_call<void>(gPatterns.C_Ctx__EndUpdateAddr, this);
    }
} // namespace SDK