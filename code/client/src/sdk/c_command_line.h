#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace SDK {
    class C_CommandLine {
      public:
        struct Command {
            std::vector<std::string> parameters;    // 0x00 - 0x18
            uint32_t flags;                         // 0x18 - 0x1C
            uint8_t pad[4];                         // 0x1C - 0x20
            // Size: 0x20 (32 bytes per command)
        };

        std::vector<Command> m_vCommands;           // 0x00 - 0x18
        std::string m_commandLineFile;              // 0x18 - ????

      public:
        int FindCommand(const char *name) const;
        const char *GetCommandParam(int cmdIdx, int paramIdx) const;
        int GetCommandParamsCount(int cmdIdx) const;
        void ProcessCommandLine(const char *cmdLine);
        void GetCommandLineFileName(const char *str);
        void GetCommandLineFromFile(const char *filename);
    };
}
