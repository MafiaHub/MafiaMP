#pragma once

#include <utils/safe_win32.h>

#include <d3d11.h>
#include <mutex>
#include <map>
#include <string>
#include <function2.hpp>

#include <Ultralight/Ultralight.h>

#include <glm/glm.hpp>

#include "sdk.h"

namespace MafiaMP::Core::UI::Web {

    class View : public ultralight::ViewListener, ultralight::LoadListener {
      private:
        ultralight::RefPtr<ultralight::Renderer> _renderer;
        ultralight::RefPtr<ultralight::View> _internalView = nullptr;

        SDK *_sdk = nullptr;

        uint8_t *_pixelData;
        int _width;
        int _height;
        bool _d3dInitialized;
        bool _shouldDisplay = false;

        ID3D11Texture2D *_texture              = nullptr;
        ID3D11ShaderResourceView *_textureView = nullptr;
        std::recursive_mutex _renderMutex;
        ultralight::Cursor _cursor = ultralight::kCursor_Pointer;
        glm::vec2 _cursorPos {};

      private:
        void OnAddConsoleMessage(ultralight::View *, ultralight::MessageSource, ultralight::MessageLevel, const ultralight::String &, uint32_t, uint32_t, const ultralight::String &) override;
        void OnDOMReady(ultralight::View *, uint64_t, bool, const ultralight::String &) override;
        void OnWindowObjectReady(ultralight::View *, uint64_t, bool, const ultralight::String &) override;
        void OnChangeCursor(ultralight::View *caller, ultralight::Cursor cursor) override;

        void InitD3D();
        void ResetTextures();
        void LoadCursorData(ultralight::Cursor cursor);

      public:
        View(ultralight::RefPtr<ultralight::Renderer>);
        ~View();

        bool Init(std::string &, int, int);

        void Update();
        void Render();
        void RenderCursor();

        void ProcessMouseEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        void ProcessKeyboardEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        void Focus(bool enable) {
            if (!_internalView) {
                return;
            }

            if (enable) {
                _internalView->Focus();
                ShowCursor(true);
            }
            else {
                _internalView->Unfocus();
                ShowCursor(false);
            }
        }

        void Display(bool enable) {
            _shouldDisplay = enable;
        }

        inline void AddEventListener(std::string eventName, const EventCallbackProc& proc) {
            if (!_sdk) {
                return;
            }

            _sdk->AddEventListener(eventName, proc);
        }

        inline void RemoveEventListener(std::string eventName) {
            if (!_sdk) {
                return;
            }

            _sdk->RemoveEventListener(eventName);
        }
    };
}
