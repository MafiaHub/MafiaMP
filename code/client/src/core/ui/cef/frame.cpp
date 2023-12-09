#include "frame.h"

#include <logging/logger.h>

namespace MafiaMP::Core::UI::CEF {
    Frame::Frame(const FrameInfo &info): _info(info) {}

    Frame::~Frame() {
        // If we have an actual browser created, notify to close
        if (_browser) {
            _browser->GetHost()->CloseBrowser(true);
        }
    }

    bool Frame::Initialize() {
        // Window informations
        CefWindowInfo info;
        info.SetAsWindowless(nullptr);

        // Browser Settings
        CefBrowserSettings settings;
        settings.javascript_close_windows    = STATE_DISABLED;
        settings.javascript_access_clipboard = STATE_DISABLED;
        settings.windowless_frame_rate       = 240;
        settings.webgl                       = STATE_ENABLED;
        settings.local_storage               = STATE_ENABLED;

        // Create our render handler
        _renderHandler = new RenderHandler(*this);

        // Create our browser implementation
        const auto ctx = CefRequestContext::GetGlobalContext();
        return CefBrowserHost::CreateBrowser(info, this, _info.url, settings, nullptr, ctx);
    }

    void Frame::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
        _browser = browser;
    };

    void Frame::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
        _browser = nullptr;
    }

    bool Frame::DoClose(CefRefPtr<CefBrowser> browser) {
        _browser = nullptr;
        return false;
    }

    bool Frame::OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString &msg, const CefString &source, int line) {
        Framework::Logging::GetLogger("CEF")->trace("OnConsoleMessage: {} {}", source.ToString().c_str(), msg.ToString().c_str());
        return false;
    }

    bool Frame::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId processId, CefRefPtr<CefProcessMessage> message) {
        return false;
    }

    void Frame::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<::CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) {
        model->Clear();
    }
} // namespace MafiaMP::Core::UI::CEF
