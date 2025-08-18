#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include "sdk/patterns.h"
#include "sdk/ue/sys/filesystem/c_virtual_file_system.h"
#include "sdk/ue/sys/filesystem/c_virtual_file_system_cache.h"

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

typedef __int64(__fastcall *C_VirtualFileSystemCache__AddDirectoryContentToCache_t)(SDK::ue::sys::filesystem::C_VirtualFileSystemCache *, const char *);
C_VirtualFileSystemCache__AddDirectoryContentToCache_t C_VirtualFileSystemCache__AddDirectoryContentToCache_original = nullptr;

__int64 __fastcall C_VirtualFileSystemCache__AddDirectoryContentToCache(SDK::ue::sys::filesystem::C_VirtualFileSystemCache *pThis, const char *path) {
    // Framework::Logging::GetLogger("VFS")->trace("Adding directory content to cache: {}", path);
    return C_VirtualFileSystemCache__AddDirectoryContentToCache_original(pThis, path);
}

typedef void *(__fastcall *C_VirtualFileSystem__Open_t)(SDK::ue::sys::filesystem::C_VirtualFileSystem *, char const *, unsigned int, char const *, unsigned char);
C_VirtualFileSystem__Open_t C_VirtualFileSystem__Open_original = nullptr;

void *__fastcall C_VirtualFileSystem__Open(SDK::ue::sys::filesystem::C_VirtualFileSystem *pThis, char const *path, unsigned int a2, char const *a3, unsigned char a4) {
    //Framework::Logging::GetLogger("VFS")->trace("Opening file: {}", path);
    return C_VirtualFileSystem__Open_original(pThis, path, a2, a3, a4);
}

static InitFunction init([]() {
        MH_CreateHook((LPVOID)SDK::gPatterns.C_VirtualFileSystemCache__AddDirectoryContentToCache, (PBYTE)C_VirtualFileSystemCache__AddDirectoryContentToCache, reinterpret_cast<void **>(&C_VirtualFileSystemCache__AddDirectoryContentToCache_original));
        MH_CreateHook((LPVOID)SDK::gPatterns.C_VirtualFileSystem__Open, (PBYTE)C_VirtualFileSystem__Open, reinterpret_cast<void **>(&C_VirtualFileSystem__Open_original));
    },
    "VFS");
