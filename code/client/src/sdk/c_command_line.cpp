#include "c_command_line.h"

#include "patterns.h"

namespace SDK {
    int C_CommandLine::FindCommand(const char *name) const {
        return hook::this_call<int>(gPatterns.C_CommandLine__FindCommand, this, name);
    }
    const char *C_CommandLine::GetCommandParam(int cmdIdx, int paramIdx) const {
        return hook::this_call<const char *>(gPatterns.C_CommandLine__GetCommandParam, this, cmdIdx, paramIdx);
    }
    int C_CommandLine::GetCommandParamsCount(int cmdIdx) const {
        return hook::this_call<int>(gPatterns.C_CommandLine__GetCommandParamsCount, this, cmdIdx);
    }
    void C_CommandLine::ProcessCommandLine(const char *cmdLine) {
        hook::this_call<void>(gPatterns.C_CommandLine__ProcessCommandLine, this, cmdLine);
    }
    void C_CommandLine::GetCommandLineFileName(const char *str) {
        hook::this_call<void>(gPatterns.C_CommandLine__GetCommandLineFileName, this, str);
    }
    void C_CommandLine::GetCommandLineFromFile(const char *filename) {
        hook::this_call<void>(gPatterns.C_CommandLine__GetCommandLineFromFile, this, filename);
    }
}
