#pragma once

#include "../../../patterns.h"
#include "c_virtual_file_system.h"

namespace SDK::ue::sys::filesystem {
    class C_VirtualFileSystemCache: public C_VirtualFileSystem {
      public:
        static C_VirtualFileSystemCache *GetInstance() {
            return hook::this_call<C_VirtualFileSystemCache *>(gPatterns.I_VirtualFileSystemCache__GetInstance);
        }
    };
} // namespace SDK::ue::sys::filesystem
