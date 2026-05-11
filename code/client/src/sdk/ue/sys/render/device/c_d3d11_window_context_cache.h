#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <stdint.h>

namespace SDK {
    namespace ue::sys::render::device {
        class I_DeviceTexture;

        // Cache of D3D11 swap chains keyed by HWND. Created lazily when a render
        // destination first targets a given window. Layout taken from
        // C_D3D11WindowContextCache::CreateWindowContext (M1DE 0x143B066E0) and
        // InitSwapChainInternal (M1DE 0x143B18750). The struct is exactly 0x60 bytes
        // (operator_new(96)).
        class C_D3D11WindowContextCache {
          public:
            struct S_WndContextDesc {
                void *_vftable;                     // 0x00 - 0x08  (off_144FAEAB8 in M1DE)
                uint32_t m_nWidth;                  // 0x08 - 0x0C
                uint32_t m_nHeight;                 // 0x0C - 0x10
                bool m_bFullscreen;                 // 0x10 - 0x11  (a5->flags & 1)
                bool m_bCreateDefaultBackBuffer;    // 0x11 - 0x12  ((a5->flags & 2) == 0)
                char _pad0[0x2];                    // 0x12 - 0x14
                int32_t m_nUnknown14;               // 0x14 - 0x18  (default -1)
                HWND m_hWindow;                     // 0x18 - 0x20
                IDXGISwapChain *m_pSwapChain;       // 0x20 - 0x28  (populated by InitSwapChainInternal)
                IDXGIOutput *m_pOutput;             // 0x28 - 0x30  (set via swap_chain->GetContainingOutput in InitSwapChainInternal)
                ID3D11Texture2D *m_pBackBufferTex;  // 0x30 - 0x38
                ID3D11ShaderResourceView *m_pBackBufferSRV;     // 0x38 - 0x40
                ID3D11UnorderedAccessView *m_pBackBufferUAV;    // 0x40 - 0x48
                ID3D11RenderTargetView *m_pBackBufferRTV;       // 0x48 - 0x50
                I_DeviceTexture *m_pDeviceTexture;  // 0x50 - 0x58  (engine wrapper around backbuffer)
                uint32_t m_nRefCount;               // 0x58 - 0x5C
                char _pad1[0x4];                    // 0x5C - 0x60
            };
            static_assert(sizeof(S_WndContextDesc) == 0x60, "S_WndContextDesc size is invalid");
        };
    } // namespace ue::sys::render::device
} // namespace SDK
