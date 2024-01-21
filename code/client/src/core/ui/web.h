#pragma once
#include <utils/safe_win32.h>

#include <d3d11.h>

#include <mutex>

#include <Ultralight/Ultralight.h>

namespace MafiaMP::Core::UI {
    class Web {
      private:
        ultralight::RefPtr<ultralight::Renderer> _renderer;
        ultralight::RefPtr<ultralight::View> _view;
        ID3D11Texture2D *_texture = nullptr;
        ID3D11ShaderResourceView *_textureView = nullptr;
        std::recursive_mutex _renderMutex;
        bool _d3dInit = false;
        uint8_t *_pixelData = nullptr;

        int _width = 0;
        int _height = 0;

        void InitD3D();

      public:
        bool Init();

        void Update();
        void Render();

        void ProcessMouseEvent(HWND, UINT, WPARAM, LPARAM) const;
        void ProcessKeyboardEvent(HWND, UINT, WPARAM, LPARAM) const;
    };
}
