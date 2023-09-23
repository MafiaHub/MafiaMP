#include "scheme_handler.h"

namespace MafiaMP::Core::UI::CEF {
    CefRefPtr<CefResourceHandler> SchemeHandlerFactory::Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString &scheme_name, CefRefPtr<CefRequest> request) {
        // TODO: register custom
        return CefRefPtr<CefResourceHandler>();
    }
} // namespace MafiaMP::Core::UI::CEF
