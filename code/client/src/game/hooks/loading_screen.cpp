#include <MinHook.h>
#include <Windows.h>
#include <Shlwapi.h>
#include <ctime>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

HBITMAP WINAPI LoadBitmapA_Hook(HINSTANCE hInstance, LPCSTR lpBitmapName) {
    return static_cast<HBITMAP>(LoadImage(NULL, "files/data/splash.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
}

typedef HFILE(__cdecl *CreateFileA_t)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

CreateFileA_t CreateFileA__Original = nullptr;
HFILE __cdecl CreateFileA__Hook(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
    if (strstr(lpFileName, "cine_loading_screen")) {
        TCHAR szPath[MAX_PATH] = {0};
        GetModuleFileName(NULL, szPath, MAX_PATH);
        PathRemoveFileSpec(szPath);

        srand(time(NULL));
        const auto randomLoading = rand() % 7 + 1;

        sprintf_s(szPath, MAX_PATH, "%s\\files\\data\\loading\\loading0%d.bik", szPath, randomLoading);
        return CreateFileA__Original(szPath, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }

    return CreateFileA__Original(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

static InitFunction init([]() {
    // Hook the splashscreen
    MH_CreateHook((LPVOID)&LoadBitmapA, (PBYTE)&LoadBitmapA_Hook, nullptr);

    // Hook the loading screen
    MH_CreateHook((LPVOID)&CreateFileA, (PBYTE)&CreateFileA__Hook, reinterpret_cast<void **>(&CreateFileA__Original));
});
