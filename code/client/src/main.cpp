#include <MinHook.h>
#include <Windows.h>
#include <logging/logger.h>
#include <utils/string_utils.h>

#include <utils/hooking/hooking_patterns.h>
#include <utils/hooking/hook_function.h>

#include "sdk/patterns.h"

extern "C" void __declspec(dllexport) InitClient(const wchar_t *projectPath) {
    Framework::Logging::GetInstance()->SetLogName("MafiaMP");
    Framework::Logging::GetInstance()->SetLogFolder(Framework::Utils::StringUtils::WideToNormal(projectPath) + "\\logs");

    MH_Initialize();
	hook::set_base();

    // Load the patterns
    SDK::Patterns::InitPatterns();

    // Entry point is handled by an InitFunction, so we just have to enable hooks and trigger the shits down here
    InitFunction::RunAll();
    MH_EnableHook(MH_ALL_HOOKS);
}
