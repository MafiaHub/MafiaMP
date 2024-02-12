#pragma once

#include "c_database_system.h"

namespace SDK {
    namespace ue::sys::sodb {
        class C_SysODB: public C_DatabaseSystem {
          public:
            static C_SysODB *GetInstance() {
                return *hook::call<C_SysODB **>(gPatterns.C_SysODB__GetInstance);
            }
        };
    }; // namespace ue::sys::sodb
};     // namespace SDK
