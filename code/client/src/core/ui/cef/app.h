#pragma once

#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/cef_client.h>

#include <vector>

namespace MafiaMP::Core::UI {
    class Web;
}

namespace MafiaMP::Core::UI::CEF {
    class Application
        : public CefApp
        , public CefRenderProcessHandler
        , public CefV8Handler {
      private:
        Web &_manager;

        using JSEventHandler = std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value>>;
        std::vector<JSEventHandler> _eventHandlers;

      public:
        Application(Web &);

        // CefApp
        void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar>) override;
        void OnBeforeCommandLineProcessing(const CefString &, CefRefPtr<CefCommandLine>) override;

        // CefV8Handler
        void InvokeEvent(const std::string &, const std::string &);
        bool Execute(const CefString &, CefRefPtr<CefV8Value>, const CefV8ValueList &, CefRefPtr<CefV8Value> &, CefString &) override;

        // CefRenderProcessHandler
        void OnContextCreated(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context>) override;
        void OnContextReleased(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context>) override;

        CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override {
            return this;
        };

        IMPLEMENT_REFCOUNTING(Application);
    };
} // namespace MafiaMP::Core::UI::CEF
