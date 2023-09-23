#pragma once

#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/cef_client.h>

#include <vector>

namespace MafiaMP::ClientWorker {
    class Application : public CefApp, CefRenderProcessHandler, CefV8Handler {
      private:
        using V8EventHandler = std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value>>;
        std::vector<V8EventHandler> _eventHandlers;

      public:
        virtual ~Application() = default;

        void CallEvent(const CefV8ValueList &args);

        // CefV8Handler
        bool Execute(const CefString &, CefRefPtr<CefV8Value>, const CefV8ValueList &, CefRefPtr<CefV8Value> &, CefString &) override;

        // CefApp
        void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar>) override;
        void OnBeforeCommandLineProcessing(const CefString &, CefRefPtr<CefCommandLine>) override;

        // CefRenderProcessHandler
        bool OnProcessMessageReceived(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefProcessId, CefRefPtr<CefProcessMessage>) override;
        void OnContextCreated(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context>) override;
        void OnContextReleased(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context>) override;

        CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override {
            return this;
        }

        IMPLEMENT_REFCOUNTING(Application);
    };
} // namespace MafiaMP::ClientWorker
