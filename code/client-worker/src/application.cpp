#include "application.h"

namespace MafiaMP::ClientWorker {
    void Application::CallEvent(const CefV8ValueList &args) {
        // Notify each event handler of the event
        for (const auto &handler : _eventHandlers) {
            handler.second->ExecuteFunctionWithContext(handler.first, nullptr, args);
        }
    }

    bool Application::Execute(const CefString &name, CefRefPtr<CefV8Value> object, const CefV8ValueList &arguments, CefRefPtr<CefV8Value> &retval, CefString &exception) {
        // TODO: add definition for register event handler and calling event
        return true;
    }

    void Application::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> reg) {
        // TODO: Register our custom scheme
    }

    void Application::OnBeforeCommandLineProcessing(const CefString &, CefRefPtr<CefCommandLine> cmdl) {
        cmdl->InitFromString(::GetCommandLineW());
    }

    bool Application::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) {
        // TODO register our call event processor

        return false;
    }

    void Application::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context> context) {
        auto window = context->GetGlobal();

        // TODO: register our event processor
    }

    void Application::OnContextReleased(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context> context) {
        if (_eventHandlers.empty())
            return;

        // Destroy each of the event handlers
        auto it = _eventHandlers.begin();
        while (it != _eventHandlers.end()) {
            if (it->first->IsSame(context))
                it = _eventHandlers.erase(it);
            else
                ++it;
        }
    }

} // namespace MafiaMP::ClientWorker
