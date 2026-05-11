#pragma once

#include "c_d3d11_window_context_cache.h"
#include "i_render_device.h"
#include "s_device_adapter_desc.h"

#include <d3d11.h>
#include <dxgi.h>

namespace SDK {
    namespace ue::sys::render::device {
        // M1DE Direct3D11 implementation of I_RenderDevice. Field layout reverse-engineered
        // from C_Direct3D11RenderDevice::Init (0x143B17D50) and its accessors
        // (GetBackBufferWidth/Height/Format, GetNumAdapters, GetDisplayAspect).
        class C_Direct3D11RenderDevice: public I_RenderDevice {
          public:
            // 0x00 - 0x08  vtable (from I_RenderDevice)
            uint32_t m_nBackBufferWidth;            // 0x08 - 0x0C
            uint32_t m_nBackBufferHeight;           // 0x0C - 0x10
            uint32_t m_nCurrentAdapterIdx;          // 0x10 - 0x14
            uint32_t m_nCurrentVideoModeIdx;        // 0x14 - 0x18
            char _pad0[0x4];                        // 0x18 - 0x1C
            bool m_bHasMainMonitor;                 // 0x1C - 0x1D  (gates GetDisplayAspect fast-path)
            bool m_bForceSoftwareAdapter;           // 0x1D - 0x1E  (use WARP)
            char _pad1[0x3];                        // 0x1E - 0x21
            bool m_bUnknownFlag21;                  // 0x21 - 0x22  (set from D3D11 feature query)
            bool m_bUnknownFlag22;                  // 0x22 - 0x23
            char _pad2[0x5];                        // 0x23 - 0x28
            uint32_t m_nUnknown28;                  // 0x28 - 0x2C
            uint32_t m_nUnknown2C;                  // 0x2C - 0x30
            char _pad3[0x78];                       // 0x30 - 0xA8
            S_DeviceAdapterDesc *m_pAdaptersBegin;      // 0xA8 - 0xB0  (std::vector<S_DeviceAdapterDesc>)
            S_DeviceAdapterDesc *m_pAdaptersEnd;        // 0xB0 - 0xB8
            S_DeviceAdapterDesc *m_pAdaptersCapacity;   // 0xB8 - 0xC0
            IDXGIFactory *m_pDxgiFactory;           // 0xC0 - 0xC8
            D3D_FEATURE_LEVEL m_eFeatureLevel;      // 0xC8 - 0xCC
            char _pad4[0x4];                        // 0xCC - 0xD0
            ID3D11Device *m_pDevice;                // 0xD0 - 0xD8
            ID3D11DeviceContext *m_pImmediateContext;   // 0xD8 - 0xE0
            char _pad5[0xA8];                       // 0xE0 - 0x188
            C_D3D11WindowContextCache *m_pWindowContextCache;   // 0x188 - 0x190
            bool m_bForceWARP;                      // 0x190 - 0x191  (true after fallback to fake adapter)

            // Convenience accessors mirroring the virtuals so we can call them without
            // bouncing through the (potentially mis-FLIRT-named) vtable in IDA.
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
