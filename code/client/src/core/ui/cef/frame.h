#pragma once

#include <atomic>
#include <string>

#include <glm/vec2.hpp>
#include <include/cef_browser.h>
#include <include/cef_client.h>

#include "render_handler.h"

namespace MafiaMP::Core::UI::CEF {
    struct FrameInfo {
        std::string url;
        uint32_t sizeX = 0;
        uint32_t sizeY = 0;

        glm::vec2 position {0};
    };

    class Frame
        : public CefClient
        , public CefLifeSpanHandler
        , public CefDisplayHandler
        , public CefContextMenuHandler {
      private:
        FrameInfo _info;
        std::atomic<bool> _visible = true;

        CefRefPtr<RenderHandler> _renderHandler;
        CefRefPtr<CefBrowser> _browser;

      public:
        Frame(const FrameInfo &info);
        ~Frame();

        bool Initialize();

        inline const FrameInfo &GetInfo() const {
            return _info;
        }

        void SetVisible(bool b) {
            _visible = b;
        };

        inline const bool IsVisible() const {
            return _visible;
        };

        inline void Render() {
            _renderHandler->Render();
        }

        // Cef Overrides
        void OnAfterCreated(CefRefPtr<CefBrowser>) override;
        void OnBeforeClose(CefRefPtr<CefBrowser>) override;
        bool DoClose(CefRefPtr<CefBrowser>) override;
        bool OnConsoleMessage(CefRefPtr<CefBrowser>, cef_log_severity_t, const CefString &, const CefString &, int) override;
        bool OnProcessMessageReceived(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefProcessId, CefRefPtr<CefProcessMessage>) override;
        void OnBeforeContextMenu(CefRefPtr<CefBrowser>, CefRefPtr<::CefFrame>, CefRefPtr<CefContextMenuParams>, CefRefPtr<CefMenuModel>) override;

        IMPLEMENT_REFCOUNTING(Frame);
    };
} // namespace MafiaMP::Core::UI::CEF
