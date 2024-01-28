#include "c_database_system.h"

#include "c_database_interface.h"

namespace SDK {
    namespace ue::sys::sodb {
        C_DatabaseInterface *C_DatabaseSystem::GetDatabase(const ue::sys::utils::C_HashName *databaseName) {
            return hook::this_call<C_DatabaseInterface *>(gPatterns.C_DatabaseSystem__GetDatabase, this, databaseName);
        }
    }; // namespace ue::sys::sodb
};     // namespace SDK
