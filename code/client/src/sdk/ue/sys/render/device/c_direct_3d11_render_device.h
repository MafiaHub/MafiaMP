#pragma once

#include "c_d3d11_window_context_cache.h"
#include "i_render_device.h"
#include "s_device_adapter_desc.h"

#include <d3d11.h>
#include <dxgi.h>

namespace SDK {
    namespace ue::sys::render::device {
        // M1DE Direct3D11 implementation of I_RenderDevice. Field layout reverse-engineered
        // from C_Direct3D11RenderDevice::Init (0x143B17D50), the renamed accessors
        // (GetBackBufferWidth at +0x08, GetBackBufferHeight at +0x0C) and the M3 Mac
        // authoritative names. Init copies S_RenderDeviceDesc verbatim:
        //   *(__m128 *)(this+8)  = desc[0..15]   -> 4 u32s
        //   *(u64    *)(this+24) = desc[16..23]  -> u8 + 5 bools + 2 pad
        class C_Direct3D11RenderDevice: public I_RenderDevice {
          public:
            // 0x00 - 0x08  vftable (inherited from I_RenderDevice)
            uint32_t m_nBackBufferWidth;            // 0x08 - 0x0C   (<- desc.m_nBackBufferWidth)
            uint32_t m_nBackBufferHeight;           // 0x0C - 0x10   (<- desc.m_nBackBufferHeight)
            uint32_t m_nCurrentAdapterIdx;          // 0x10 - 0x14   (<- desc.m_nAdapterIdx, read by GetFrequency)
            uint32_t m_nCurrentVideoModeIdx;        // 0x14 - 0x18   (<- desc.m_nVideoModeIdx, read by GetFrequency)
            uint8_t m_eSwapEffect;                  // 0x18 - 0x19   (<- desc.m_eSwapEffect)
            bool m_bFlagBit01;                      // 0x19 - 0x1A   (<- desc.m_bFlagBit01)
            bool m_bFlagBit12;                      // 0x1A - 0x1B   (<- desc.m_bFlagBit12)
            bool m_bFlagBit15;                      // 0x1B - 0x1C   (<- desc.m_bFlagBit15)
            bool m_bFullscreen;                     // 0x1C - 0x1D   (<- desc.m_bFullscreen, gates GetFrequency cached-mode path)
            bool m_bAllowSWFallback;                // 0x1D - 0x1E   (<- desc.m_bFlagBit16, gates fake-WARP-adapter creation in Init)
            char _pad0[0x3];                        // 0x1E - 0x21
            bool m_bD3D11OptionFlag8;               // 0x21 - 0x22   (Init: CheckFeatureSupport(D3D11_OPTIONS).v34[9])
            bool m_bD3D11OptionFlag7;               // 0x22 - 0x23   (Init: CheckFeatureSupport(D3D11_OPTIONS).v34[8])
            char _pad1[0x5];                        // 0x23 - 0x28
            uint32_t m_nGlobalFlags;                // 0x28 - 0x2C   (= dword_145B09F6C in Init)
            uint32_t m_nUnknown2C;                  // 0x2C - 0x30
            char _pad2[0x8];                        // 0x30 - 0x38
            void *m_pGPUVendorSpecific;             // 0x38 - 0x40   (returned by slot 8 GetGPUVendorSpecific)
            char _pad3[0x68];                       // 0x40 - 0xA8
            S_DeviceAdapterDesc *m_pAdaptersBegin;      // 0xA8 - 0xB0  (std::vector<S_DeviceAdapterDesc>)
            S_DeviceAdapterDesc *m_pAdaptersEnd;        // 0xB0 - 0xB8
            S_DeviceAdapterDesc *m_pAdaptersCapacity;   // 0xB8 - 0xC0
            IDXGIFactory *m_pDxgiFactory;           // 0xC0 - 0xC8
            D3D_FEATURE_LEVEL m_eFeatureLevel;      // 0xC8 - 0xCC
            char _pad4[0x4];                        // 0xCC - 0xD0
            ID3D11Device *m_pDevice;                // 0xD0 - 0xD8
            ID3D11DeviceContext *m_pImmediateContext;   // 0xD8 - 0xE0
            char _pad5[0xA8];                       // 0xE0 - 0x188
            C_D3D11WindowContextCache *m_pWindowContextCache;   // 0x188 - 0x190  (read by OnRenderDestinationCreated)
            bool m_bForceWARP;                      // 0x190 - 0x191  (set when Init falls back to the fake software adapter)

            // Convenience accessors (non-virtual, read cached fields directly — they do
            // NOT go through the binary vtable, so they're safe even though the C++ class
            // doesn't declare its virtuals).
            inline ID3D11Device *GetDevice() const { return m_pDevice; }
            inline ID3D11DeviceContext *GetImmediateContext() const { return m_pImmediateContext; }
            inline IDXGIFactory *GetDxgiFactory() const { return m_pDxgiFactory; }
            inline C_D3D11WindowContextCache *GetWindowContextCache() const { return m_pWindowContextCache; }

            inline bool IsHardwareDevice() const { return !m_bForceWARP; }

            inline uint32_t GetCurrentBackBufferWidth() const  { return m_nBackBufferWidth; }
            inline uint32_t GetCurrentBackBufferHeight() const { return m_nBackBufferHeight; }

            inline uint32_t GetAdapterCount() const {
                return static_cast<uint32_t>(m_pAdaptersEnd - m_pAdaptersBegin);
            }

            inline const S_DeviceAdapterDesc *GetAdapter(uint32_t i) const {
                return m_pAdaptersBegin + i;
            }
        };
    } // namespace ue::sys::render::device
} // namespace SDK
