#include <Windows.h>

#include <external/cef/worker.hpp>

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    return Framework::External::CEF::WorkerMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
