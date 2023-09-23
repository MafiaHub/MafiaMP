#include "app.h"

namespace MafiaMP::Core::UI::CEF {
    Application::Application(Web &mgr): _manager(mgr) {}

    void Application::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> reg) {
        // TODO: register our custom scheme
    }

    void Application::OnBeforeCommandLineProcessing(const CefString &, CefRefPtr<CefCommandLine> cmdl) {
        cmdl->AppendSwitch("enable-experimental-web-platform-features");
        cmdl->AppendSwitch("transparent-painting-enabled");
        cmdl->AppendSwitch("off-screen-rendering-enabled");
    }

    void Application::InvokeEvent(const std::string &name, const std::string &data) {
        CefV8ValueList args;
        CefRefPtr<CefV8Value> retval;
        args.push_back(CefV8Value::CreateString(name));
        args.push_back(CefV8Value::CreateString(data));

        for (auto &handler : _eventHandlers) {
            handler.second->ExecuteFunctionWithContext(handler.first, nullptr, args);
        }
    }

    bool Application::Execute(const CefString &name, CefRefPtr<CefV8Value>, const CefV8ValueList &args, CefRefPtr<CefV8Value> &, CefString &) {
        // TODO: bind our builtin event functions
        return true;
    }

    void Application::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context> ctx) {
        CefRefPtr<CefV8Value> window = ctx->GetGlobal();

        // TODO: bind our builtin event functions
    }

    void Application::OnContextReleased(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context> ctx) {
        if (_eventHandlers.empty()) {
            return;
        }

        // Destroy our event handlers
        auto it = _eventHandlers.begin();
        while (it != _eventHandlers.end()) {
            if (it->first->IsSame(ctx)) {
                it = _eventHandlers.erase(it);
            }
            else {
                ++it;
            }
        }
    }
} // namespace MafiaMP::Core::UI::CEF
