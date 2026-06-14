#pragma once

#include <cstdint>

namespace SDK {
    class I_TickedModuleCallEventContext {
      public:
        virtual ~I_TickedModuleCallEventContext()   = default;
        virtual void *GetInterface()                = 0;
        virtual float *GetDeltaTime()               = 0;
        virtual bool RunNextAction(int flags)       = 0;
        virtual bool IsFinished()                   = 0;
        virtual void *YieldCurrentAction()          = 0;

        void **m_pActionList;       // 0x08 - 0x10
        void *m_pDeltaTimePtr;      // 0x10 - 0x18
        uint32_t m_nActionIndex;    // 0x18 - 0x1C
        uint32_t m_nState;          // 0x1C - 0x20
        void *m_pCurrentAction;     // 0x20 - 0x28
        int32_t m_nFlags;           // 0x28 - 0x2C
        char _pad2C[0x4];           // 0x2C - 0x30
        void *m_pTimeBudgetInfo;    // 0x30 - 0x38
        void *m_pCoreInterface;     // 0x38 - 0x40
    };
    static_assert(sizeof(I_TickedModuleCallEventContext) == 0x40, "I_TickedModuleCallEventContext size mismatch");
} // namespace SDK
