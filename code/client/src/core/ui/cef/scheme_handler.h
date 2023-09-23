#pragma once

#include <include/cef_scheme.h>

namespace MafiaMP::Core::UI::CEF {
    class SchemeHandlerFactory: public CefSchemeHandlerFactory {
      public:
        CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString &scheme_name, CefRefPtr<CefRequest> request) override;

        IMPLEMENT_REFCOUNTING(SchemeHandlerFactory);
    };

} // namespace MafiaMP::Core::UI::CEF
