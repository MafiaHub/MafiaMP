#pragma once

#include "i_render_device.h"
#include "c_d3d11_window_context_cache.h"

#include <d3d11.h>

namespace SDK {
    namespace ue::sys::render::device {
        class C_Direct3D11RenderDevice: public I_RenderDevice {
          public:
            char pad0[0xC0];                                    // 0000 - 00C0
            IDXGIAdapter *m_pDxgiAdapter;                       // 00C0 - 00C8
            D3D_FEATURE_LEVEL m_pFeatureLevel;                  // 00C8 - 00D0
            ID3D11Device *m_pDevice;                            // 00D0 - 00D8
            ID3D11DeviceContext *m_pImmediateContext;           // 00D8 - 00E0
            char pad1[0xA8];                                    // 00E0 - 0188
            C_D3D11WindowContextCache *m_pWindowContextCache;   // 0188 - 0190
            bool m_bIsHardwareDevice;                           // 0190 - 0191

            inline bool IsHardwareDevice() const {
                return m_bIsHardwareDevice;
            }

            inline IDXGIAdapter *GetDxgiAdapter() const {
                return m_pDxgiAdapter;
            }

            inline ID3D11Device* GetDevice() const {
                return m_pDevice;
            }

            inline ID3D11DeviceContext* GetImmediateContext() const {
                return m_pImmediateContext;
            }

            inline C_D3D11WindowContextCache *GetWindowContextCache() const {
                return m_pWindowContextCache;
            }
        };
    } // namespace ue::sys::render::device
} // namespace SDK
