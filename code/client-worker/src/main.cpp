#include <application.h>

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    CefMainArgs args(hInstance);
    return CefExecuteProcess(args, new MafiaMP::ClientWorker::Application, nullptr);
}
