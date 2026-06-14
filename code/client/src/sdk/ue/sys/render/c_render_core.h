#pragma once

#include "device/i_render_device.h"
#include "s_graphics_init_desc.h"

#include <stdint.h>

namespace SDK {
    namespace ue::sys::render {
        namespace device {
            class C_DynamicVIBufferPool;
            class I_RenderDeviceCtx;
        }
        class C_RenderContext;

        // Top-level rendering façade owned by C_Core (m_pRenderCore). Sets up the
        // render device, creates the default render destination, and caches a number
        // of values queried from the device via the virtual interface.
        // Offsets and cache slots reverse-engineered from C_RenderCore::Init
        // (M1DE 0x143AB76C0) and CreateDefaultRenderDestination (M1DE 0x143A913A0).
        class C_RenderCore {
          public:
            char _pad0[0x8];                                    // 0x000 - 0x008  (vtable)
            void *m_pRenderBackend;                             // 0x008 - 0x010
            void *m_pUnknown10;                                 // 0x010 - 0x018
            void *m_pSettingsListener;                          // 0x018 - 0x020
            char _pad1[0x8];                                    // 0x020 - 0x028
            void *m_pPipeline;                                  // 0x028 - 0x030
            void *m_pUnknown30;                                 // 0x030 - 0x038
            device::I_RenderDevice *m_pRenderDevice;            // 0x038 - 0x040
            device::I_RenderDeviceCtx *m_pPrimaryCtx;           // 0x040 - 0x048
            device::I_RenderDeviceCtx *m_pAsyncCtx;             // 0x048 - 0x050
            device::C_DynamicVIBufferPool *m_pDynamicVIBufferPool;   // 0x050 - 0x058
            void *m_pFrameSink;                                 // 0x058 - 0x060
            C_RenderContext *m_pDefaultRenderContext;           // 0x060 - 0x068
            char _pad2[0xF8];                                   // 0x068 - 0x160
            char _pad3[0xA0];                                   // 0x160 - 0x200

            // Cached values written by C_RenderCore::Init from device-vtable calls.
            uint32_t m_nDisplayWidth;                           // 0x200 - 0x204  (= S_GraphicsInitDesc.m_nDisplayWidth)
            uint32_t m_nDisplayHeight;                          // 0x204 - 0x208  (= S_GraphicsInitDesc.m_nDisplayHeight)
            uint32_t m_nFrequency;                              // 0x208 - 0x20C  (<- vtable slot 18 GetFrequency)
            uint32_t m_eBackBufferFormat;                       // 0x20C - 0x210  (<- vtable slot 17 GetBackBufferFormat, M1DE always 21)
            float m_fDisplayAspect;                             // 0x210 - 0x214  (<- vtable slot 19 GetDisplayAspect)

            // 20 bytes copied from the monitor-desc helper (vtable slot 22, M3:
            // C_Direct3D11RenderDevice::GetMainMonitorDesc). The helper builds a static
            // buffer of {0, 0, BackBufferWidth, BackBufferHeight, DisplayAspect}.
            uint32_t m_nMonDescPad0;                            // 0x214 - 0x218
            uint32_t m_nMonDescPad1;                            // 0x218 - 0x21C
            uint32_t m_nCachedBackBufferWidth;                  // 0x21C - 0x220  (real cached width)
            uint32_t m_nCachedBackBufferHeight;                 // 0x220 - 0x224  (real cached height)
            float m_fCachedDisplayAspect;                       // 0x224 - 0x228  (duplicate of m_fDisplayAspect)

            uint32_t m_nVTable23Result;                         // 0x228 - 0x22C  (<- vtable slot 23, M1DE returns 1)
            uint32_t m_nVTable24Result;                         // 0x22C - 0x230  (<- vtable slot 24, M1DE returns 1)
            bool m_bVTable25Result;                             // 0x230 - 0x231  (<- vtable slot 25, M1DE returns false)
            char _pad6[0x1F];                                   // 0x231 - 0x250
            uint8_t m_bUseAsyncDevice;                          // 0x250 - 0x251  (mirrors S_GraphicsInitDesc.m_bAsyncRenderDevice)
            char _pad7[0xF];                                    // 0x251 - 0x260
            void *m_pDeviceCallbacks;                           // 0x260 - 0x268

            inline device::I_RenderDevice *GetRenderDevice() const { return m_pRenderDevice; }
            inline uint32_t GetDisplayWidth() const { return m_nDisplayWidth; }
            inline uint32_t GetDisplayHeight() const { return m_nDisplayHeight; }
            inline uint32_t GetBackBufferWidth() const { return m_nCachedBackBufferWidth; }
            inline uint32_t GetBackBufferHeight() const { return m_nCachedBackBufferHeight; }
            inline float GetDisplayAspect() const { return m_fDisplayAspect; }
            inline uint32_t GetFrequency() const { return m_nFrequency; }
            inline uint32_t GetBackBufferFormat() const { return m_eBackBufferFormat; }
        };
    } // namespace ue::sys::render
} // namespace SDK
