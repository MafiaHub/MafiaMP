#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include "vfs.h"

namespace MafiaMP::Core::Hooks {
    SDK::ue::sys::filesystem::C_VirtualFileSystemCache* MafiaMPVFS::Construct() {
        // Call the original constructor
        const auto ctorPattern = hook::get_opcode_address("E8 ? ? ? ? 48 8B F0 EB 03 48 8B F5 48 8B 06");
        hook::this_call(ctorPattern, this);

        Framework::Logging::GetLogger("Hooks")->debug("MafiaMP VFS constructed");

        // Store the global instance

        return this;
    }

    bool MafiaMPVFS::Initialize(const S_Init& init) {
        // Call the original method, this will also trigger the constructor hook we installed
        bool result            = hook::call<bool>(0x000000143CE4560, &init);

        Framework::Logging::GetLogger("Hooks")->debug("MafiaMP VFS initialized = {}", result);

        return result;
    }

    void MafiaMPVFS::Install() {
        hook::call(0x000000143CE45A3, hook::bind<uintptr_t>(&MafiaMPVFS::Construct));
        hook::call(0x00000014240B9E9, hook::bind<uintptr_t>(&MafiaMPVFS::Initialize));
    }
}

static InitFunction init([]() {
    // MafiaMP::Core::Hooks::MafiaMPVFS::Install();
    },
    "VFS");
