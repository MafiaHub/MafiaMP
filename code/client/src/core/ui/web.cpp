#include "web.h"

#include <AppCore/Platform.h>
#include <logging/logger.h>

#include "../../game/module.h"

#include <Inc/SimpleMath.h>

namespace MafiaMP::Core::UI {
    bool Web::Init() {
        // Prepare the configuration object
        ultralight::Config config;
        config.resource_path = "./resources/";
        config.use_gpu_renderer = false;
        config.device_scale = 1.0;

        // Create our GPU driver
        const auto renderDevice = Game::gGlobals.renderDevice;
        if (!renderDevice) {
            Framework::Logging::GetLogger("UI")->error("Failed to grab game render device");
            return false;
        }
        _device = renderDevice->GetDevice();
        _context = renderDevice->GetImmediateContext();
        _immediateContext = renderDevice->GetImmediateContext();

        if (!_device || !_context || !_immediateContext) {
            Framework::Logging::GetLogger("UI")->error("Failed to grab game render device or context ({} {})", fmt::ptr(_device.Get()), fmt::ptr(_context.Get()));
            return false;
        }

        _spriteBatch = std::make_unique<DirectX::SpriteBatch>(_immediateContext.Get());
        _states      = std::make_unique<DirectX::CommonStates>(_device.Get());

        // Configure the platform via the singleton
        ultralight::Platform::instance().set_config(config);
        ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());
        ultralight::Platform::instance().set_file_system(ultralight::GetPlatformFileSystem("."));
        ultralight::Platform::instance().set_logger(ultralight::GetDefaultLogger("ultralight.log"));

        // Create our renderer
        _renderer = ultralight::Renderer::Create();
        if (!_renderer || !_renderer.get()) {
            Framework::Logging::GetLogger("UI")->error("Failed to create renderer");
            return false;
        }

        Framework::Logging::GetLogger("UI")->debug("Web Renderer initialized");

        return true;
    }

    bool Web::CreateTexture(uint32_t width, uint32_t height) {
        D3D11_TEXTURE2D_DESC textDesc;
        textDesc.Width     = width;
        textDesc.Height    = height;
        textDesc.MipLevels = textDesc.ArraySize = 1;
        textDesc.Format                         = DXGI_FORMAT_B8G8R8A8_UNORM;
        textDesc.SampleDesc.Count               = 1;
        textDesc.SampleDesc.Quality             = 0;
        textDesc.Usage                          = D3D11_USAGE_DYNAMIC;
        textDesc.BindFlags                      = D3D11_BIND_SHADER_RESOURCE;
        textDesc.CPUAccessFlags                 = D3D11_CPU_ACCESS_WRITE;
        textDesc.MiscFlags                      = 0;

        HRESULT result = _device->CreateTexture2D(&textDesc, nullptr, _texture.ReleaseAndGetAddressOf());
        if (FAILED(result)) {
            Framework::Logging::GetLogger("UI")->error("Failed to create 2D Texture");
            return false;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC sharedResourceViewDesc = {};
        sharedResourceViewDesc.Format                          = textDesc.Format;
        sharedResourceViewDesc.ViewDimension                   = D3D11_SRV_DIMENSION_TEXTURE2D;
        sharedResourceViewDesc.Texture2D.MipLevels             = 1;

        if (FAILED(_device->CreateShaderResourceView(_texture.Get(), &sharedResourceViewDesc, _textureView.ReleaseAndGetAddressOf()))) {
            Framework::Logging::GetLogger("UI")->error("Failed to create shader resource view");
            return false;
        }

        return true;
    }

    bool Web::CreateView(std::string name, int width, int height, std::string url) {
        ultralight::RefPtr<ultralight::View> view = _renderer->CreateView(width, height, false, nullptr);
        if (!view || !view.get()) {
            Framework::Logging::GetLogger("UI")->error("Failed to create view");
            return false;
        }

        view->LoadURL(ultralight::String(url.c_str()));
        view->Focus();

        _views.emplace(name, view);
        Framework::Logging::GetLogger("UI")->debug("Created view {}", name.c_str());
        return true;
    }

    bool Web::ToggleViewFocus(std::string name) {
        const auto view = _views[name];
        if (!view || !view.get()) {
            Framework::Logging::GetLogger("UI")->error("Failed to find view");
            return false;
        }

        if (view->HasFocus()) {
            view->Unfocus();
        }
        else {
            view->Focus();
        }
        return true;
    }

    void Web::ConvertPixelsToARGB(uint8_t *pixels, uint32_t width, uint32_t height, uint32_t stride) {
        for (uint32_t y = 0; y < height; y++) {
            for (uint32_t x = 0; x < width; x++) {
                uint8_t *pixel = pixels + (y * stride) + (x * 4);
                std::swap(pixel[0], pixel[2]);
            }
        }
    }

    void Web::CopyPixelsToTexture(void *pixels, uint32_t width, uint32_t height, uint32_t stride) {
        if (!_texture || !_textureView) {
            if (!CreateTexture(width, height)) {
                __debugbreak();
            }
        }

        // Paint to the texture
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        const auto mapResult = _context->Map(_texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        if (!SUCCEEDED(mapResult)) {
            _context.Reset();
            _texture.Reset();
            return;
        }
        const auto pDest = static_cast<uint8_t *>(mappedResource.pData);
        std::memcpy(pDest, pixels, width * height * 4);
        _context->Unmap(_texture.Get(), 0);

        // Process command lists
        Microsoft::WRL::ComPtr<ID3D11CommandList> commandsList;
        const auto result = _context->FinishCommandList(FALSE, &commandsList);

        if (result == S_OK && commandsList) {
            _immediateContext->ExecuteCommandList(commandsList.Get(), TRUE);
        }

        _spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, _states->NonPremultiplied());
        _spriteBatch->Draw(_textureView.Get(), DirectX::SimpleMath::Vector2(0.f, 0.f), nullptr, DirectX::Colors::White, 0.f);
        _spriteBatch->End();
    }

    void Web::CopyBitmapToTexture(ultralight::RefPtr<ultralight::Bitmap> bitmap) {
        void *pixels = bitmap->LockPixels();

        uint32_t width  = bitmap->width();
        uint32_t height = bitmap->height();
        uint32_t stride = bitmap->row_bytes();

        CopyPixelsToTexture(pixels, width, height, stride);

        bitmap->UnlockPixels();
    }

    void Web::Update() {
        if (!_renderer || !_renderer.get()) {
            return;
        }

        _renderer->Update();
    }

    void Web::Render() {
        if (!_renderer || !_renderer.get()) {
            return;
        }

        _renderer->Render();

        for (auto view : _views) {
            // Acquire the bitmap surface
            ultralight::BitmapSurface *surface = (ultralight::BitmapSurface *)(view.second->surface());

            // Check if the surface is dirty (pixels changed)
            if (!surface->dirty_bounds().IsEmpty()) {
                // Copy the bitmap to our GPU texture
                CopyBitmapToTexture(surface->bitmap());

                // Mark the surface as clean
                surface->ClearDirtyBounds();
            }
        }
    }
}
