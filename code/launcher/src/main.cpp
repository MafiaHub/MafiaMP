#include <Windows.h>

#include <launcher/project.h>

#pragma comment(linker, "/merge:.data=.cld")
#pragma comment(linker, "/merge:.rdata=.clr")
#pragma comment(linker, "/merge:.cl=.zdata")
#pragma comment(linker, "/merge:.text=.zdata")
#pragma comment(linker, "/section:.zdata,re")

#pragma bss_seg(".mmp")
char guest_seg[0x14000000];

char stub_seg[0x100000];

#pragma data_seg(".zdata")
char zdata[200] = {1};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    Framework::Launcher::ProjectConfiguration config;
    config.destinationDllName = L"MafiaMPClient.dll";
    config.executableName     = L"mafiadefinitiveedition.exe";
    config.name               = "MafiaMP";
    config.platform           = Framework::Launcher::ProjectPlatform::STEAM;
    config.steamAppId         = 1030840;

    Framework::Launcher::Project project(config);
    if (!project.Launch()) {
        return 1;
    }
    return 0;
}
