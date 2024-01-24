#pragma once

#include "../utils/c_hash_name.h"

namespace SDK {
    namespace ue::sys::sodb {
        class C_DatabaseInterface;

        class C_DatabaseSystem {
          public:
            C_DatabaseInterface *GetDatabase(const ue::sys::utils::C_HashName *databaseName);
        };
    }; // namespace ue::sys::sodb
};     // namespace SDK
