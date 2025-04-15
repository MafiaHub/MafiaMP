#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "sdk/ue/c_string.h"

namespace SDK {
    class C_CommandLine {
      public:
        struct Command {
            std::vector<ue::C_String> parameters; // 0x00-0x18
            void *padding[3];                     // 0x18-0x20
        }; // Size: 0x20 (32 bytes)

        ue::C_String m_CommandLine;       // 0x00-0x08
        std::vector<Command> m_vCommands; // 0x08-0x20
        ue::C_String m_CommandFile;       // 0x20-0x28

      public:
        int FindCommand(const char *name) const;
        const char *GetCommandParam(int cmdIdx, int paramIdx) const;
        int GetCommandParamsCount(int cmdIdx) const;
        void ProcessCommandLine(const char *cmdLine);
        void GetCommandLineFileName(const char *str);
        void GetCommandLineFromFile(const char *filename);
    };
}
