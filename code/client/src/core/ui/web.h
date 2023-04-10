#pragma once
#include <utils/safe_win32.h>
#include <ultralight/Ultralight.h>

#include <string>
#include <unordered_map>

#include <wrl/client.h>
#include <d3d11.h>
#include <Inc/SpriteBatch.h>
#include <Inc/CommonStates.h>

namespace MafiaMP::Core::UI {
    class Web {
      private:
        Microsoft::WRL::ComPtr<ID3D11Device> _device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> _immediateContext;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _textureView;

        std::unique_ptr<DirectX::SpriteBatch> _spriteBatch;
        std::unique_ptr<DirectX::CommonStates> _states;

      protected:
        ultralight::RefPtr<ultralight::Renderer> _renderer;
        std::unordered_map<std::string, ultralight::RefPtr<ultralight::View>> _views;

      public:
        bool Init();
        bool CreateView(std::string name, int width, int height, std::string url);
        bool ToggleViewFocus(std::string name);

        void Update();
        void Render();

      private:
        bool CreateTexture(uint32_t width, uint32_t height);
        void CopyBitmapToTexture(ultralight::RefPtr<ultralight::Bitmap>);
        void CopyPixelsToTexture(void *pixels, uint32_t width, uint32_t height, uint32_t stride);
        void ConvertPixelsToARGB(uint8_t *pixels, uint32_t width, uint32_t height, uint32_t stride);
    };
}
