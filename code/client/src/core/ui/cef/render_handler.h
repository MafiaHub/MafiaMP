#pragma once

#include <mutex>

#include <Inc/CommonStates.h>
#include <Inc/SpriteBatch.h>
#include <d3d11.h>
#include <wrl/client.h>

#include <include/cef_browser.h>
#include <include/cef_client.h>

namespace MafiaMP::Core::UI::CEF {
    class Frame;

    class RenderHandler: public CefRenderHandler {
      private:
        Frame &_frame;

        std::mutex _textureMutex;

        Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> _immediateContext;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _textureView;

        std::unique_ptr<DirectX::SpriteBatch> _spriteBatch;
        std::unique_ptr<DirectX::CommonStates> _states;

      public:
        RenderHandler(Frame &frame);
        ~RenderHandler();

        bool Initialize();

        void Render();
        void Reset();

        // CEF Overrides
        void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
        void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

      private:
        bool CreateTexture(); // Note: Called on the main thread
        bool UpdateTexture(); // Note: Called on the render thread

        IMPLEMENT_REFCOUNTING(RenderHandler);
    };
} // namespace MafiaMP::Core::UI::CEF
