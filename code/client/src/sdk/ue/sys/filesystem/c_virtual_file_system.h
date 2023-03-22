#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../utils/c_hash_name.h"
#include "../../c_string.h"

namespace SDK::ue {
    enum class E_Level : int64_t {

    };
}

namespace SDK::ue::sys::filesystem {
    class I_VirtualFileSystemCallbacks;

    class C_MountLayerConfiguration;

    class C_FileSystemInfo;

    class I_File;

    enum class E_FileSystemEntryType : int64_t {

    };

    enum class FileAccessMode : int64_t {

    };

    class I_VirtualFileSystem {
        // TODO: pad ?
      public:
        virtual ~I_VirtualFileSystem() = default;

        virtual int64_t Init(E_Level);
        virtual int64_t Done();
        virtual int64_t GetRealDir(char const *, ue::C_String &);
        virtual int64_t GetMountPoint(char const *, ue::C_String &);
        virtual int64_t GetRealPath(char const *, ue::C_String &, char const *);
        virtual const char *GetPhysicalPaths(char const *, char const *);
        virtual const char *Mount(char const *, char const *);
        virtual int64_t Mount(C_MountLayerConfiguration &);
        virtual int64_t Unmount(char const *);
        virtual bool UnmountAll();
        virtual int64_t GetLayerLayout();
        virtual int64_t GetLastModificationTime(char const *);
        virtual int64_t DeleteVFS(char const *, char const *);
        virtual int64_t IsReadOnly(char const *, char const *);
        virtual int64_t SetReadOnly(char const *, bool);
        virtual int64_t CreateDirectory(char const *, char const *);
        virtual int64_t VFSPathToSWDAndFile(char const *, ue::C_String &, ue::C_String &);
        virtual int64_t EnumFiles(char const *);
        virtual int64_t EnumFilesWildcard(char const *, char const *, bool);
        virtual int64_t EnumerateFileSystemEntries(std::string const &, std::vector<ue::sys::filesystem::C_FileSystemInfo> &, bool, ue::sys::filesystem::E_FileSystemEntryType, char const *);
        virtual int64_t GetFileSystemInfos(std::string const &, std::vector<ue::sys::filesystem::C_FileSystemInfo> &, bool, char const *);
        virtual int64_t GetSearchPath();
        virtual int64_t FreeList(void *);
        virtual bool IsEmpty(char const *);
        virtual bool IsDirectory(char const *);
        virtual bool IsDirectoryFromEnumFiles(char const *);
        virtual bool Exists(char const *, ue::sys::filesystem::E_FileSystemEntryType);
        virtual I_File *Open(char const *, ue::sys::filesystem::FileAccessMode, char const *, unsigned char);
        virtual void LogAssetList(bool);
        // TODO: unk1
        virtual int64_t SearchForPathFromRoot(ue::sys::utils::C_HashName const &, char const *);
        // TODO: unk2
        virtual void ReportMissingFile(char const *);
        virtual int64_t SearchForFileFromRoot(char const *);
        virtual int64_t TrySearchForFileFromRoot(char const *);
        // TODO: unk3
        virtual int64_t Close(ue::sys::filesystem::I_File *);
        //TODO: continue
    };

    class C_VirtualFileSystem : public I_VirtualFileSystem {

    };
}
