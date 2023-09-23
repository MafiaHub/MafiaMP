#include <utils/safe_win32.h>

#include <Inc/SimpleMath.h>

#include "../../../game/module.h"
#include "frame.h"
#include "render_handler.h"

namespace MafiaMP::Core::UI::CEF {
    RenderHandler::RenderHandler(Frame &frame): _frame(frame) {}

    RenderHandler::~RenderHandler() {
        // Release our texture
        std::lock_guard<std::mutex> lockTexture(_textureMutex);
        if (_texture) {
            _texture.Reset();
        }

        if (_textureView) {
            _textureView.Reset();
        }

        // Release our custom contexts
        _context.Reset();
        _immediateContext.Reset();
    }

    bool RenderHandler::Initialize() {
        // Grab our game builtin device
        const auto renderDevice = Game::gGlobals.renderDevice;
        if (!renderDevice || !renderDevice->_device) {
            return false;
        }

        // Grab our immediate context
        renderDevice->_device->GetImmediateContext(_immediateContext.ReleaseAndGetAddressOf());
        if (!_immediateContext) {
            return false;
        }

        // Create a new dedicated context
        if (FAILED(renderDevice->_device->CreateDeferredContext(0, _context.ReleaseAndGetAddressOf()))) {
            return false;
        }

        // Create our sprites
        _spriteBatch = std::make_unique<DirectX::SpriteBatch>(_immediateContext.Get());
        _states      = std::make_unique<DirectX::CommonStates>(renderDevice->_device);

        // Create our texture
        if (!_texture && !CreateTexture()) {
            return false;
        }

        return true;
    }

    bool RenderHandler::CreateTexture() {
        const auto renderDevice = Game::gGlobals.renderDevice;
        if (!renderDevice || !renderDevice->_device) {
            return false;
        }

        auto &info = _frame.GetInfo();

        D3D11_TEXTURE2D_DESC textDesc;
        textDesc.Width     = info.sizeX;
        textDesc.Height    = info.sizeY;
        textDesc.MipLevels = textDesc.ArraySize = 1;
        textDesc.Format                         = DXGI_FORMAT_B8G8R8A8_UNORM;
        textDesc.SampleDesc.Count               = 1;
        textDesc.SampleDesc.Quality             = 0;
        textDesc.Usage                          = D3D11_USAGE_DYNAMIC;
        textDesc.BindFlags                      = D3D11_BIND_SHADER_RESOURCE;
        textDesc.CPUAccessFlags                 = D3D11_CPU_ACCESS_WRITE;
        textDesc.MiscFlags                      = 0;

        if (FAILED(renderDevice->_device->CreateTexture2D(&textDesc, nullptr, _texture.ReleaseAndGetAddressOf()))) {
            return false;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC sharedResourceViewDesc = {};
        sharedResourceViewDesc.Format                          = textDesc.Format;
        sharedResourceViewDesc.ViewDimension                   = D3D11_SRV_DIMENSION_TEXTURE2D;
        sharedResourceViewDesc.Texture2D.MipLevels             = 1;

        if (FAILED(renderDevice->_device->CreateShaderResourceView(_texture.Get(), &sharedResourceViewDesc, _textureView.ReleaseAndGetAddressOf()))) {
            _texture->Release();
            _texture = nullptr;
            return false;
        }

        return true;
    }

    bool RenderHandler::UpdateTexture() {
        // Make sure we have a texture
        if (!_texture && !CreateTexture()) {
            return false;
        }

        // TODO

        return true;
    }

    void RenderHandler::Render() {
        // Make sure we can update the texture
        std::lock_guard<std::mutex> lock(_textureMutex);
        if (!UpdateTexture()) {
            return;
        }

        // Grab the information
        const auto &info = _frame.GetInfo();

        // Grab the render device
        if (!_context || !_immediateContext) {
            return;
        }

        // Process command lists
        std::unique_lock textureLock(_textureMutex);

        Microsoft::WRL::ComPtr<ID3D11CommandList> commandsList;
        const auto result = _context->FinishCommandList(FALSE, &commandsList);

        if (result == S_OK && commandsList) {
            _immediateContext->ExecuteCommandList(commandsList.Get(), TRUE);
        }

        // Render if intended
        if (_frame.IsVisible()) {
            _spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, _states->NonPremultiplied());

            std::unique_lock<std::mutex> textureLock(_textureMutex);

            if (_textureView) {
                _spriteBatch->Draw(_textureView.Get(), DirectX::SimpleMath::Vector2(0.f, 0.f), nullptr, DirectX::Colors::White, 0.f);
            }

            // TODO: Render cursor

            _spriteBatch->End();
        }
    }
    void RenderHandler::Reset() {
        // Release our texture
        std::lock_guard<std::mutex> lock(_textureMutex);
        if (_texture) {
            _texture.Reset();
        }
        if (_textureView) {
            _textureView.Reset();
        }

        // Release our context
        _context.Reset();
        _immediateContext.Reset();

        // Create everything again
        Initialize();
    }

    void RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) {
        const auto &info = _frame.GetInfo();
        rect             = CefRect(0, 0, info.sizeX, info.sizeY);
    }

    void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &rects, const void *buffer, int width, int height) {
        const auto &info = _frame.GetInfo();

        if (type == PET_VIEW && info.sizeX == width && info.sizeY == height) {
            std::unique_lock textureLock(_textureMutex);

            if (!_texture)
                CreateTexture();

            D3D11_MAPPED_SUBRESOURCE mappedResource;
            const auto result = _context->Map(_texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

            if (SUCCEEDED(result)) {
                const auto pDest = static_cast<uint8_t *>(mappedResource.pData);
                std::memcpy(pDest, buffer, width * height * 4);
                _context->Unmap(_texture.Get(), 0);
            }
            else {
                _texture.Reset();
            }
        }
    }
} // namespace MafiaMP::Core::UI::CEF
