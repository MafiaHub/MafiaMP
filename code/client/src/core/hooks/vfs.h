#pragma once

#include "sdk/ue/sys/filesystem/c_virtual_file_system_cache.h"

struct S_Init {

};

namespace MafiaMP::Core::Hooks {
    class MafiaMPVFS : public SDK::ue::sys::filesystem::C_VirtualFileSystemCache {
        SDK::ue::sys::filesystem::C_VirtualFileSystemCache *Construct(); // Do not call directly

        static bool Initialize(const S_Init &);

        public:
            MafiaMPVFS() = default;
            // ~MafiaMPVFS() = delete;

            static void Install();
    };
}
