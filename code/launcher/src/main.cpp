#include <Windows.h>

#include <launcher/project.h>

int main(void) {
    Framework::Launcher::ProjectConfiguration config;
    config.destinationDllName = L"MafiaMPClient.dll";
    config.executableName     = L"mafiadefinitiveedition.exe";
    config.name               = "MafiaMP";
    config.platforms          = {std::make_shared<Framework::Launcher::Platforms::Steam>(Framework::Launcher::Platforms::Steam::Options {.appId = 1030840})};
    config.verifyGameIntegrity = true;
    config.supportedGameVersions = {3168979183};

#ifdef FW_DEBUG
    config.allocateDeveloperConsole = true;
    config.developerConsoleTitle = L"mafiamp: dev-console";
#endif

    Framework::Launcher::Project project(config);
    if (!project.Launch()) {
        return 1;
    }
    return 0;
}
