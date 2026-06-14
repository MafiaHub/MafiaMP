#pragma once

#include "i_ticked_module_call_event_context.h"

#include <cstdint>

namespace SDK {

    class C_TickedModule {
      public:
        virtual ~C_TickedModule()                       = default;
        virtual uint64_t GetClassTypeInfo() const { return 0; }
        virtual uint64_t GetTypeInfo() const { return 0; }
        virtual const char *GetTickedModuleName() { return ""; }
        virtual void StaticRegister() {}
        virtual float GetTimeBudget() const { return 0.005f; }
    };
} // namespace SDK
