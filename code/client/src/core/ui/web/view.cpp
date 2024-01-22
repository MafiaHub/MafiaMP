#include "view.h"

#include "../../../game/module.h"

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <stdio.h>
#ifdef _MSC_VER
#pragma comment(lib, "d3dcompiler") // Automatically link with d3dcompiler.lib as we are using D3DCompile() below.
#endif

struct WebData {
    ID3D11Buffer *pVB;
    ID3D11Buffer *pIB;
    ID3D11VertexShader *pVertexShader;
    ID3D11InputLayout *pInputLayout;
    ID3D11Buffer *pVertexConstantBuffer;
    ID3D11PixelShader *pPixelShader;
    ID3D11SamplerState *pSampler;
    ID3D11RasterizerState *pRasterizerState;
    ID3D11BlendState *pBlendState;
    ID3D11DepthStencilState *pDepthStencilState;

    WebData() {
        memset(this, 0, sizeof(*this));
    }
};

WebData *bd = new WebData;

struct VERTEX_CONSTANT_BUFFER {
    float mvp[4][4];
};

struct VERTEX {
    float x, y;
    float u, v;
};

namespace MafiaMP::Core::UI::Web {
    View::View(ultralight::RefPtr<ultralight::Renderer> renderer): _renderer(renderer) {}

    bool View::Init(std::string &path, int width, int height) {
        // Initialize a view configuration
        ultralight::ViewConfig config;
        config.is_accelerated = false;
        config.is_transparent = true;
        config.initial_focus  = false;

        // Initialize the internal view
        _internalView = _renderer->CreateView(width, height, config, nullptr);
        if (!_internalView || !_internalView.get()) {
            return false;
        }

        // Bind the listeners to the internal view
        _internalView->set_view_listener(this);
        _internalView->set_load_listener(this);

        // Load the initial URL
        _internalView->LoadURL(path.c_str());

        // Store the width/height
        _width = width;
        _height = height;
        return true;
    }

    void View::InitD3D() {
        // No need to initialize twice
        if (_d3dInitialized) {
            return;
        }

        // Acquire the rendering devices from the application
        const auto device    = gApplication->GetRenderer()->GetD3D11Backend()->GetDevice();
        const auto renderCtx = gApplication->GetRenderer()->GetD3D11Backend()->GetContext();
        if (!device || !renderCtx) {
            return;
        }

        {
            static const char *vertexShader = "cbuffer vertexBuffer : register(b0) \
            {\
              float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
              float2 pos : POSITION;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
              PS_INPUT output;\
              output.pos = float4(input.pos.xy, 0.f, 1.f);\
              output.uv  = input.uv;\
              return output;\
            }";

            ID3DBlob *vertexShaderBlob;
            if (FAILED(D3DCompile(vertexShader, strlen(vertexShader), NULL, NULL, NULL, "main", "vs_4_0", 0, 0, &vertexShaderBlob, NULL)))
                return; // NB: Pass ID3DBlob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
            if (device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), NULL, &bd->pVertexShader) != S_OK) {
                vertexShaderBlob->Release();
                return;
            }

            // Create the input layout
            D3D11_INPUT_ELEMENT_DESC local_layout[] = {
                {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, (UINT)0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, (UINT)8, D3D11_INPUT_PER_VERTEX_DATA, 0},
            };
            if (device->CreateInputLayout(local_layout, 2, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &bd->pInputLayout) != S_OK) {
                vertexShaderBlob->Release();
                return;
            }
            vertexShaderBlob->Release();

            // Create the constant buffer
            {
                D3D11_BUFFER_DESC desc;
                desc.ByteWidth      = sizeof(VERTEX_CONSTANT_BUFFER);
                desc.Usage          = D3D11_USAGE_DYNAMIC;
                desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                desc.MiscFlags      = 0;
                device->CreateBuffer(&desc, NULL, &bd->pVertexConstantBuffer);
            }
        }

        // Create the pixel shader
        {
            static const char *pixelShader = "struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float2 uv  : TEXCOORD0;\
            };\
            sampler sampler0;\
            Texture2D texture0;\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
            float4 out_col = texture0.Sample(sampler0, input.uv); \
            return out_col; \
            }";

            ID3DBlob *pixelShaderBlob;
            if (FAILED(D3DCompile(pixelShader, strlen(pixelShader), NULL, NULL, NULL, "main", "ps_4_0", 0, 0, &pixelShaderBlob, NULL)))
                return; // NB: Pass ID3DBlob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
            if (device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), NULL, &bd->pPixelShader) != S_OK) {
                pixelShaderBlob->Release();
                return;
            }
            pixelShaderBlob->Release();
        }

        // Create the blending setup
        {
            D3D11_BLEND_DESC desc;
            ZeroMemory(&desc, sizeof(desc));
            desc.AlphaToCoverageEnable                 = false;
            desc.RenderTarget[0].BlendEnable           = true;
            desc.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
            desc.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
            desc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
            desc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
            desc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_INV_SRC_ALPHA;
            desc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
            desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            device->CreateBlendState(&desc, &bd->pBlendState);
        }

        // Create the rasterizer state
        {
            D3D11_RASTERIZER_DESC desc;
            ZeroMemory(&desc, sizeof(desc));
            desc.FillMode        = D3D11_FILL_SOLID;
            desc.CullMode        = D3D11_CULL_NONE;
            desc.ScissorEnable   = true;
            desc.DepthClipEnable = true;
            device->CreateRasterizerState(&desc, &bd->pRasterizerState);
        }

        // Create depth-stencil State
        {
            D3D11_DEPTH_STENCIL_DESC desc;
            ZeroMemory(&desc, sizeof(desc));
            desc.DepthEnable             = false;
            desc.DepthWriteMask          = D3D11_DEPTH_WRITE_MASK_ALL;
            desc.DepthFunc               = D3D11_COMPARISON_ALWAYS;
            desc.StencilEnable           = false;
            desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            desc.FrontFace.StencilFunc                                                                      = D3D11_COMPARISON_ALWAYS;
            desc.BackFace                                                                                   = desc.FrontFace;
            device->CreateDepthStencilState(&desc, &bd->pDepthStencilState);
        }

        // Create vertex/index buffers
        D3D11_BUFFER_DESC desc;
        memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
        desc.Usage          = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth      = 6 * sizeof(VERTEX);
        desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags      = 0;
        if (device->CreateBuffer(&desc, NULL, &bd->pVB) < 0)
            return;

        memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
        desc.Usage          = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth      = 6 * sizeof(unsigned);
        desc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        if (device->CreateBuffer(&desc, NULL, &bd->pIB) < 0)
            return;

        // Upload vertex/index data into a single contiguous GPU buffer
        D3D11_MAPPED_SUBRESOURCE vtx_resource, idx_resource;
        if (renderCtx->Map(bd->pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK)
            return;
        if (renderCtx->Map(bd->pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK)
            return;

        static VERTEX vertices[] = {
            // First triangle
            {-1.0f, -1.0f, 0.0f, 1.0f}, // Bottom left
            {1.0f, -1.0f, 1.0f, 1.0f},  // Bottom right
            {-1.0f, 1.0f, 0.0f, 0.0f},  // Top left

            // Second triangle
            {1.0f, -1.0f, 1.0f, 1.0f}, // Bottom right
            {1.0f, 1.0f, 1.0f, 0.0f},  // Top right
            {-1.0f, 1.0f, 0.0f, 0.0f}, // Top left
        };

        static unsigned short indices[] = {
            0, 1, 2, // Indices for the first triangle
            3, 4, 5  // Indices for the second triangle
        };

        VERTEX *vtx_dst   = (VERTEX *)vtx_resource.pData;
        unsigned *idx_dst = (unsigned *)idx_resource.pData;
        memcpy(vtx_dst, vertices, 6 * sizeof(VERTEX));
        memcpy(idx_dst, indices, 6 * sizeof(unsigned short));
        renderCtx->Unmap(bd->pVB, 0);
        renderCtx->Unmap(bd->pIB, 0);

        // Create texture sampler
        {
            D3D11_SAMPLER_DESC desc;
            ZeroMemory(&desc, sizeof(desc));
            desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
            desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
            desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
            desc.MipLODBias     = 0.f;
            desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
            desc.MinLOD         = 0.f;
            desc.MaxLOD         = 0.f;
            device->CreateSamplerState(&desc, &bd->pSampler);
        }

        _d3dInitialized = true;
    }

    void View::ResetTextures() {
        std::lock_guard lock(_renderMutex);

        // Grab the device & context
        const auto device    = gApplication->GetRenderer()->GetD3D11Backend()->GetDevice();
        const auto renderCtx = gApplication->GetRenderer()->GetD3D11Backend()->GetContext();

        if (!device || !renderCtx) {
            return;
        }

        // TODO: Destroy the texture if window size has changed
        if (!_texture) {
            D3D11_TEXTURE2D_DESC textureDesc;
            ZeroMemory(&textureDesc, sizeof(textureDesc));
            textureDesc.Width              = _width;  // width of the texture
            textureDesc.Height             = _height; // height of the texture
            textureDesc.MipLevels          = 1;
            textureDesc.ArraySize          = 1;
            textureDesc.Format             = DXGI_FORMAT_B8G8R8A8_UNORM; // texture format
            textureDesc.SampleDesc.Count   = 1;
            textureDesc.SampleDesc.Quality = 0;
            textureDesc.Usage              = D3D11_USAGE_DEFAULT;
            textureDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE; // bind to shader
            textureDesc.CPUAccessFlags     = 0;
            textureDesc.MiscFlags          = 0;

            // Step 2: Create the texture
            device->CreateTexture2D(&textureDesc, nullptr, &_texture);

            // Step 3: Create a shader-resource view
            if (FAILED(device->CreateShaderResourceView(_texture, nullptr, &_textureView))) {
                Framework::Logging::GetLogger("Web")->error("Failed to create texture view");
                return;
            }
        }

        D3D11_BOX destRegion;
        destRegion.left       = 0;
        destRegion.right      = _width;
        destRegion.top        = 0;
        destRegion.bottom     = _height;
        destRegion.front      = 0;
        destRegion.back       = 1;
        const auto rowPitch   = _width * 4;
        const auto depthPitch = rowPitch * _height;

        renderCtx->UpdateSubresource(_texture, 0, &destRegion, _pixelData, rowPitch, depthPitch);
    }

    void View::Update() {
        if (!_internalView || !_internalView.get()) {
            return;
        }

        std::lock_guard lock(_renderMutex);

        auto surface  = (ultralight::BitmapSurface *)_internalView->surface();
        void *pixels = surface->LockPixels();
        int size     = surface->size(); // TODO: calc from res
        // TODO: Realloc if size changes
        if (!_pixelData) {
            _pixelData = new uint8_t[size];
        }
        memcpy(_pixelData, pixels, size);
        surface->UnlockPixels();
    }

    void View::Render() {
        std::lock_guard lock(_renderMutex);

        // Make sure we have D3D setup
        InitD3D();

        // Make sure we have textures
        if (!_pixelData) {
            return;
        }

        // Grab the device & context
        const auto device    = gApplication->GetRenderer()->GetD3D11Backend()->GetDevice();
        const auto renderCtx = gApplication->GetRenderer()->GetD3D11Backend()->GetContext();

        // Reset the textures
        ResetTextures();

        if (!_texture || !_textureView) {
            return;
        }

        // Render ye
        struct BACKUP_DX11_STATE {
            UINT ScissorRectsCount, ViewportsCount;
            D3D11_RECT ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
            D3D11_VIEWPORT Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
            ID3D11RasterizerState *RS;
            ID3D11BlendState *BlendState;
            FLOAT BlendFactor[4];
            UINT SampleMask;
            UINT StencilRef;
            ID3D11DepthStencilState *DepthStencilState;
            ID3D11ShaderResourceView *PSShaderResource;
            ID3D11SamplerState *PSSampler;
            ID3D11PixelShader *PS;
            ID3D11VertexShader *VS;
            ID3D11GeometryShader *GS;
            UINT PSInstancesCount, VSInstancesCount, GSInstancesCount;
            ID3D11ClassInstance *PSInstances[256], *VSInstances[256], *GSInstances[256]; // 256 is max according to PSSetShader documentation
            D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology;
            ID3D11Buffer *IndexBuffer, *VertexBuffer, *VSConstantBuffer;
            UINT IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
            DXGI_FORMAT IndexBufferFormat;
            ID3D11InputLayout *InputLayout;
        };
        BACKUP_DX11_STATE old = {};
        old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
        renderCtx->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);
        renderCtx->RSGetViewports(&old.ViewportsCount, old.Viewports);
        renderCtx->RSGetState(&old.RS);
        renderCtx->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);
        renderCtx->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);
        renderCtx->PSGetShaderResources(0, 1, &old.PSShaderResource);
        renderCtx->PSGetSamplers(0, 1, &old.PSSampler);
        old.PSInstancesCount = old.VSInstancesCount = old.GSInstancesCount = 256;
        renderCtx->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
        renderCtx->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
        renderCtx->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);
        renderCtx->GSGetShader(&old.GS, old.GSInstances, &old.GSInstancesCount);

        renderCtx->IAGetPrimitiveTopology(&old.PrimitiveTopology);
        renderCtx->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);
        renderCtx->IAGetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);
        renderCtx->IAGetInputLayout(&old.InputLayout);

        // Apply scissor/clipping rectangle
        const D3D11_RECT r = {0, 0, _width, _height};
        renderCtx->RSSetScissorRects(1, &r);

        // Setup shader and vertex buffers
        unsigned int stride = sizeof(VERTEX);
        unsigned int offset = 0;
        renderCtx->IASetInputLayout(bd->pInputLayout);
        renderCtx->IASetVertexBuffers(0, 1, &bd->pVB, &stride, &offset);
        renderCtx->IASetIndexBuffer(bd->pIB, DXGI_FORMAT_R16_UINT, 0);
        renderCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        renderCtx->VSSetShader(bd->pVertexShader, NULL, 0);
        renderCtx->VSSetConstantBuffers(0, 1, &bd->pVertexConstantBuffer);
        renderCtx->PSSetShader(bd->pPixelShader, NULL, 0);
        renderCtx->PSSetSamplers(0, 1, &bd->pSampler);
        renderCtx->GSSetShader(NULL, NULL, 0);
        renderCtx->HSSetShader(NULL, NULL, 0); // In theory we should backup and restore this as well.. very infrequently used..
        renderCtx->DSSetShader(NULL, NULL, 0); // In theory we should backup and restore this as well.. very infrequently used..
        renderCtx->CSSetShader(NULL, NULL, 0); // In theory we should backup and restore this as well.. very infrequently used..

        // Setup blend state
        const float blend_factor[4] = {0.f, 0.f, 0.f, 0.f};
        renderCtx->OMSetBlendState(bd->pBlendState, blend_factor, 0xffffffff);
        renderCtx->OMSetDepthStencilState(bd->pDepthStencilState, 0);
        renderCtx->RSSetState(bd->pRasterizerState);

        // Setup orthographic projection matrix into our constant buffer
        {
            D3D11_MAPPED_SUBRESOURCE mapped_resource;
            if (renderCtx->Map(bd->pVertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource) != S_OK)
                return;
            VERTEX_CONSTANT_BUFFER *constant_buffer = (VERTEX_CONSTANT_BUFFER *)mapped_resource.pData;
            float L                                 = 0;
            float R                                 = _width;
            float T                                 = 0;
            float B                                 = _height;
            float mvp[4][4]                         = {
                {2.0f / (R - L), 0.0f, 0.0f, 0.0f},
                {0.0f, 2.0f / (T - B), 0.0f, 0.0f},
                {0.0f, 0.0f, 0.5f, 0.0f},
                {(R + L) / (L - R), (T + B) / (B - T), 0.5f, 1.0f},
            };
            memcpy(&constant_buffer->mvp, mvp, sizeof(mvp));
            renderCtx->Unmap(bd->pVertexConstantBuffer, 0);
        }

        renderCtx->PSSetShaderResources(0, 1, &_textureView);
        renderCtx->DrawIndexed(6, 0, 0);

        // Restore modified DX state
        renderCtx->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
        renderCtx->RSSetViewports(old.ViewportsCount, old.Viewports);
        renderCtx->RSSetState(old.RS);
        if (old.RS)
            old.RS->Release();
        renderCtx->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask);
        if (old.BlendState)
            old.BlendState->Release();
        renderCtx->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef);
        if (old.DepthStencilState)
            old.DepthStencilState->Release();
        renderCtx->PSSetShaderResources(0, 1, &old.PSShaderResource);
        if (old.PSShaderResource)
            old.PSShaderResource->Release();
        renderCtx->PSSetSamplers(0, 1, &old.PSSampler);
        if (old.PSSampler)
            old.PSSampler->Release();
        renderCtx->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount);
        if (old.PS)
            old.PS->Release();
        for (UINT i = 0; i < old.PSInstancesCount; i++)
            if (old.PSInstances[i])
                old.PSInstances[i]->Release();
        renderCtx->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount);
        if (old.VS)
            old.VS->Release();
        renderCtx->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer);
        if (old.VSConstantBuffer)
            old.VSConstantBuffer->Release();
        renderCtx->GSSetShader(old.GS, old.GSInstances, old.GSInstancesCount);
        if (old.GS)
            old.GS->Release();
        for (UINT i = 0; i < old.VSInstancesCount; i++)
            if (old.VSInstances[i])
                old.VSInstances[i]->Release();
        renderCtx->IASetPrimitiveTopology(old.PrimitiveTopology);
        renderCtx->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset);
        if (old.IndexBuffer)
            old.IndexBuffer->Release();
        renderCtx->IASetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);
        if (old.VertexBuffer)
            old.VertexBuffer->Release();
        renderCtx->IASetInputLayout(old.InputLayout);
        if (old.InputLayout)
            old.InputLayout->Release();
    }

    void View::ProcessMouseEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (!_internalView) {
            return;
        }

        if (!_internalView->HasFocus()) {
            return;
        }

        // Handle the mouse wheel event as separate from the other mouse events
        if (msg == WM_MOUSEWHEEL) {
            ultralight::ScrollEvent ev;
            ev.type    = ultralight::ScrollEvent::kType_ScrollByPixel;
            ev.delta_x = 0;
            ev.delta_y = GET_WHEEL_DELTA_WPARAM(wParam) * 0.8;
            _internalView->FireScrollEvent(ev);
            return;
        }

        // Handle other classic mouse events
        ultralight::MouseEvent ev;
        ev.x = LOWORD(lParam); // TODO: revamp this because it fails on multiple monitors setups
        ev.y = HIWORD(lParam); // TODO: revamp this because it fails on multiple monitors setups
        switch (msg) {
        case WM_MOUSEMOVE: {
            ev.type   = ultralight::MouseEvent::kType_MouseMoved;
            ev.button = ultralight::MouseEvent::kButton_None;
        } break;
        case WM_LBUTTONDOWN: {
            ev.type   = ultralight::MouseEvent::kType_MouseDown;
            ev.button = ultralight::MouseEvent::kButton_Left;
        } break;
        case WM_LBUTTONUP: {
            ev.type   = ultralight::MouseEvent::kType_MouseUp;
            ev.button = ultralight::MouseEvent::kButton_Left;
        } break;
        case WM_RBUTTONDOWN: {
            ev.type   = ultralight::MouseEvent::kType_MouseDown;
            ev.button = ultralight::MouseEvent::kButton_Right;
        } break;
        case WM_RBUTTONUP: {
            ev.type   = ultralight::MouseEvent::kType_MouseUp;
            ev.button = ultralight::MouseEvent::kButton_Right;
        } break;
        case WM_MBUTTONDOWN: {
            ev.type   = ultralight::MouseEvent::kType_MouseDown;
            ev.button = ultralight::MouseEvent::kButton_Middle;
        } break;
        case WM_MBUTTONUP: {
            ev.type   = ultralight::MouseEvent::kType_MouseUp;
            ev.button = ultralight::MouseEvent::kButton_Middle;
        } break;
        }

        _internalView->FireMouseEvent(ev);
    }

    void View::ProcessKeyboardEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (!_internalView) {
            return;
        }

        if (!_internalView->HasFocus()) {
            return;
        }

        ultralight::KeyEvent ev;
        switch (msg) {
        case WM_KEYDOWN: {
            ev.type = ultralight::KeyEvent::kType_RawKeyDown;
        } break;
        case WM_KEYUP: {
            ev.type = ultralight::KeyEvent::kType_KeyUp;
        } break;
        case WM_CHAR: {
            char key[2]        = {(char)wParam, 0};
            ev.type            = ultralight::KeyEvent::kType_Char;
            ev.text            = key;
            ev.unmodified_text = ev.text;

            // Make sure that pressing enter does not trigger this event
            if (key[0] == 13) {
                return;
            }
        } break;
        }

        ev.virtual_key_code = wParam;
        ev.native_key_code  = lParam;

        ultralight::GetKeyIdentifierFromVirtualKeyCode(ev.virtual_key_code, ev.key_identifier);
        _internalView->FireKeyEvent(ev);
    }

    void View::OnAddConsoleMessage(ultralight::View *, ultralight::MessageSource source, ultralight::MessageLevel level, const ultralight::String &message, uint32_t line_number, uint32_t column_number, const ultralight::String &source_id) {
        Framework::Logging::GetLogger("Web")->debug("Console message: {}:{}:{}:{}", message.utf8().data(), line_number, column_number, source_id.utf8().data());
    }

    void View::OnDOMReady(ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url) {
        Framework::Logging::GetLogger("Web")->debug("DOM ready");
    }

    void View::OnWindowObjectReady(ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url) {
        Framework::Logging::GetLogger("Web")->debug("Window object ready");
    }
}
