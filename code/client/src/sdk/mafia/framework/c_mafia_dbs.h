#pragma once

#include <utils/hooking/hooking.h>

#include "../../ue/sys/sodb/c_sys_odb.h"

namespace SDK {
    namespace mafia::framework {
        class C_MafiaDBs {
          public:
            void GetTablesDatabase(ue::sys::sodb::C_SysODB *odb) {
                const auto pattern = hook::get_opcode_address("E8 ? ? ? ? 48 8B F8 49 8B C7");
                hook::this_call(pattern, this, odb);
            }
        };

        static C_MafiaDBs *GetMafiaDBs() {
            const auto pattern = hook::get_opcode_address("E8 ? ? ? ? 48 8D 55 87 48 8B C8");
            return hook::call<C_MafiaDBs *>(pattern);
        }
    }; // namespace mafia::framework
};     // namespace SDK
